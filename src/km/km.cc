/**
 * @brief km.cc
 *
 * @date  : Feb 12, 2016
 * @author: lpzun
 */

#include "km.hh"

namespace gkm {

GKM::GKM() {
    // TODO Auto-generated constructor stub

}

GKM::~GKM() {
    // TODO Auto-generated destructor stub
}

bool GKM::reachability_analysis_via_gkm(const string& filename,
        const string& initl, const string& final, const bool& is_self_loop) {
    this->initl_TS = utils::create_thread_state_from_str(initl);
    this->final_TS = utils::create_thread_state_from_str(final);

    if (filename == "X") {
        throw gkm_runtime_error("no input file");
    } else {
        ifstream org_in;
        if (!Refs::OPT_INPUT_TTS) {
            //org_in.open(this->parse_BP(filename).c_str());
            string line;
            std::getline(org_in, line);
        } else {
            org_in.open(filename.c_str());
        }
        if (!org_in.good())
            throw gkm_runtime_error("Input file does not find!");
        iparser::remove_comments(org_in, "/tmp/tmp.ttd.no_comment", "#");
        org_in.close();

        /// new input file after removing comments
        ifstream new_in("/tmp/tmp.ttd.no_comment");

        new_in >> thread_state::S >> thread_state::L;
        if (!Refs::OPT_INPUT_TTS)
            final_TS = thread_state(thread_state::S - 1, thread_state::L - 1);

        shared_state s1, s2;              /// shared states
        local_state l1, l2;               /// local  states
        string sep;                       /// separator
        while (new_in >> s1 >> l1 >> sep >> s2 >> l2) {
            DBG_STD(
                    cout << s1 << " " << l1 << " -> " << s2 << " " << l2 << " "
                    << transition_ID << "\n")
            if (!is_self_loop && s1 == s2 && l1 == l2) /// remove self loops
                continue;

            if (sep == "->" || sep == "+>") {
                const thread_state src_TS(s1, l1);
                const thread_state dst_TS(s2, l2);
                if (sep == "+>") {
                    orispawn_TTS[src_TS].emplace_back(dst_TS);
                }
                original_TTS[src_TS].emplace_back(dst_TS);
            } else {
                throw gkm_runtime_error("illegal transition");
            }
        }
        new_in.close();

        if (Refs::OPT_PRINT_ADJ && Refs::OPT_INPUT_TTS) {
            for (auto isrc = original_TTS.begin(); isrc != original_TTS.end();
                    ++isrc) {
                for (auto idst = isrc->second.begin();
                        idst != isrc->second.end(); ++idst) {
                    if (this->is_spawn_transition(isrc->first, *idst))
                        cout << isrc->first << "+>" << *idst << "\n";
                    else
                        cout << isrc->first << "->" << *idst << "\n";
                }
            }
            cout << endl;
        }
    }
//    if (this->is_connected())
//        return true;
    return this->standard_GKM();
}

/**
 * @brief the standard Karp-Miller procedure
 * @return true : if the final state is coverable
 *         false: otherwise
 */
bool GKM::standard_GKM() {
    antichain worklist; /// the set of states that have yet to explore
    antichain explored; /// the set of states that have been  explored
    while (!worklist.empty()) {
        const auto tau = worklist.front(); /// tau has to be a copy
        worklist.pop_front(); /// remove current state from worklist
        const auto& images = this->step(tau);
        for (const auto& _tau : images) {
            cout << _tau << "\n";
        }
    }
    return false;
}

/**
 * @brief compute all post images of tau
 * @param tau
 * @return
 */
deque<global_state> GKM::step(const global_state& tau) {
    deque<global_state> images;
    const auto& s = tau.get_share();
    for (const auto & p : tau.get_locals()) {
        const auto& l = p.first;
        thread_state curr(s, l);
        auto ifind = this->original_TTS.find(curr);
        const auto& successors = ifind->second;
        for (auto isucc = successors.begin(); isucc != successors.end();
                ++isucc) {
            const auto& succ = *isucc;
            if (this->is_spawn_transition(curr, succ)) {
                /// update counters
                const auto& _tau = this->update_counter(curr.get_share(),
                        curr.get_local(), succ.get_share(), succ.get_local(),
                        tau.get_locals(), true);
                /// omega acceleration
                const auto& wtau = this->w_acceleration(_tau);
                /// push it into images
                images.emplace_back(wtau);
            } else {

            }
        }
    }
    return images;
}

/**
 * @brief determine if (src, dst) corresponds to a spawn transition
 * @param src
 * @param dst
 * @return bool
 *          true : src +> dst
 *          false: otherwise
 */
bool GKM::is_spawn_transition(const thread_state& src,
        const thread_state& dst) {
    auto ifind = this->orispawn_TTS.find(src);
    if (ifind == this->orispawn_TTS.end()) {
        return false;
    } else {
        auto ifnd = std::find(ifind->second.begin(), ifind->second.end(), dst);
        if (ifnd == ifind->second.end())
            return false;
        else
            return true;
    }
}

/**
 * @brief update counter
 * @param s
 * @param l
 * @param _s
 * @param _l
 * @param Z
 * @param is_spawn
 * @return global state
 */
global_state GKM::update_counter(const shared_state& s, const local_state& l,
        const shared_state& _s, const local_state& _l, const ca_locals& Z,
        const bool& is_spawn) {
    auto _Z = Z;

    auto idec = _Z.find(l);
    if (idec != _Z.end() && idec->second != omega) {
        idec->second--;
        if (idec->second == 0)
            _Z.erase(idec);
    }

    auto iinc = _Z.find(_l);
    if (iinc != _Z.end()) {
        if (iinc->second != omega) {
            if (idec->second == omega && s == _s) {
                iinc->second = omega;
            } else {
                iinc->second++;
            }
        }
    } else if (idec->second == omega && s == _s) {
        _Z.emplace(_l, omega);
    } else {
        _Z.emplace(_l, 1);
    }
    return global_state(_s, _Z);
}
/**
 * @brief omega acceleration
 * @param tau: current state
 * @return a state after omega acceleration
 */
global_state GKM::w_acceleration(const global_state& tau) {

}

} /* namespace iotf */
