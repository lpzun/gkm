/**
 * @brief km.cc
 *
 * @date  : Feb 12, 2016
 * @author: lpzun
 */

#include "km.hh"

namespace gkm {

GKM::GKM() {

}

GKM::~GKM() {

}

/**
 * @brief the interface of forward reachability analysis
 * @param filename
 * @param s_initl
 * @param s_final
 * @param is_self_loop
 * @return true : if the final state is reachable;
 *         false: otherwise.
 */
bool GKM::reachability_analysis_via_gkm(const string& filename,
        const string& s_initl, const string& s_final,
        const bool& is_self_loop) {
    if (filename == "X") {
        throw gkm_runtime_error("no input file");
    } else {
        ifstream org_in;
        if (!refer::OPT_INPUT_TTS) {
            org_in.open(this->parse_BP(filename + ".tts").c_str());
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
        if (!refer::OPT_INPUT_TTS) {
            final_TS = this->set_up_TS(this->parse_BP(filename) + ".prop");
            //cout << final_TS << endl;
        } else {
            initl_TS = this->set_up_TS(s_initl);
            final_TS = this->set_up_TS(s_final);
            if (initl_TS == final_TS) { /// if initial == final, then
                return true;            /// return true immediately
            }
        }

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

        if (refer::OPT_PRINT_ADJ && refer::OPT_INPUT_TTS) {
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
 * @brief parse Boolean program: this is for testing
 * @param filename
 * @return
 */
string GKM::parse_BP(const string& filename) {
    string file = filename;
    file = file.substr(0, file.find_last_of("."));
    return file;
}

/**
 * @brief setup initial or final thread state
 * @param s_ts
 * @return thread state
 */
thread_state GKM::set_up_TS(const string& s_ts) {
    /// setup the initial thread state
    if (s_ts.find('|') != std::string::npos) {
        return utils::create_thread_state_from_str(s_ts);
    } else {
        ifstream in(s_ts.c_str());
        if (in.good()) {
            string s_io;
            std::getline(in, s_io);
            in.close();
            return utils::create_thread_state_from_str(s_io);
        } else {
            //throw bws_runtime_error("initial state file does not find!");
            return thread_state(thread_state::S - 1, thread_state::L - 1);
        }
    }
}

/**
 * @brief the standard Karp-Miller procedure
 * @return true : if the final state is coverable
 *         false: otherwise
 */
bool GKM::standard_GKM() {
    //cout << __func__ << "=================\n";
    antichain worklist; /// the set of states that have yet to explore
    antichain explored; /// the set of states that have been  explored
    deque<global_state> sigma; /// the prefix of path leads to a state

    worklist.emplace_front(initl_TS, refer::omega); /// initialize worklist
    while (!worklist.empty()) {
        const auto tau = worklist.front(); /// tau has to be  a copy
        worklist.pop_front(); /// remove current state from <worklist>

        //cout << "=================" << tau << "\n";

        /// step 1: if upward(tau) is already explored, then
        ///         discard it, like the visited-mark in DFS
        if (!this->is_maximal(tau, explored))
            continue;

        /// step 2: compute all post images; check if final
        ///         state is coverable; maximize <worklist>
        const auto& images = this->step(tau);
        if (!images.empty()) {
            /// step 2.1: update prefix of path leads to postimages
            this->update_predecessors(tau, sigma);
            /// step 2.2: process the above obtained postimages
            for (const auto& _tau : images) {
                //cout << "-------b---------" << _tau << "\n";
                /// step 1: omega acceleration for _tau
                const auto& wtau = this->w_acceleration(_tau, sigma);
                //cout << "-------a---------" << wtau << "\n";
                /// step 2: return true if wtau covers final state
                if (this->is_reached(wtau))
                    return true;
                /// step 3: discard wtau when upward(wtau) exists
                if (!this->is_maximal(wtau, worklist))
                    continue;
                /// step 4: maximize <worklist> in term of wtau
                this->maximize(wtau, worklist);
                /// step 5: insert wtau into the worklist
                worklist.emplace_front(wtau, tau.get_depth() + 1);
            }
        }
        /// step 2.3: maximize the <explored> in term of tau
        this->maximize(tau, explored);
        explored.emplace_back(tau); /// insert into explored
    }
    return false;
}

/**
 * @brief compute all post images of tau
 * @param tau
 * @return the list of post images
 */
deque<global_state> GKM::step(const global_state& tau) {
    deque<global_state> images;
    const auto& s = tau.get_share();
    for (const auto& p : tau.get_locals()) {
        const auto& l = p.first;
        thread_state curr(s, l);
        auto ifind = this->original_TTS.find(curr);
        if (ifind != this->original_TTS.end()) {
            const auto& successors = ifind->second;
            for (const auto& succ : successors) {
                if (this->is_spawn_transition(curr, succ)) {
                    /// update counters
                    const auto& _tau = this->update_counter(curr.get_share(),
                            succ.get_share(), succ.get_local(),
                            tau.get_locals());
                    /// push it into images
                    images.emplace_back(_tau);
                } else {
                    /// update counters
                    const auto& _tau = this->update_counter(curr.get_share(),
                            curr.get_local(), succ.get_share(),
                            succ.get_local(), tau.get_locals());
                    /// push it into images
                    images.emplace_back(_tau);
                }
            }
        }
    }
    return images;
}

/**
 * @brief omega acceleration procedure
 * @param tau  : current state
 * @param sigma: the prefix path leads to _tau
 * @return a state after omega acceleration
 */
global_state GKM::w_acceleration(const global_state& _tau,
        const deque<global_state>& sigma) {
    /// count all non-omega local states in _tau
    ushort nacc = 0;
    for (const auto& p : _tau.get_locals()) {
        if (p.second < refer::omega)
            ++nacc;
    }
    /// there is no local state that needs to accelerate
    if (nacc == 0)
        return _tau;

    /// collect all local states that need to accelerate
    set<local_state> wls;
    /// step 1: iterate over all preceding global states
    for (const auto& tau : sigma) {
        if (this->is_covered(tau, _tau))
            /// iterate over all local states residing in state _tau
            for (const auto& p : _tau.get_locals())
                /// if local state p.first's counter < w and it has
                /// not been accelerated, then process it
                if ((p.second < refer::omega)
                        && (wls.find(p.first) == wls.end())) {
                    auto ifind = tau.get_locals().find(p.first);
                    if (ifind != tau.get_locals().end()
                            && ifind->second < p.second) {
                        wls.emplace(p.first); ///need to accelerate
                    }
                }
        /// already collect all local states that need to accelerate
        if (wls.size() == nacc)
            break;
    }
    /// there is no way to accelerate _tau
    if (wls.size() == 0)
        return _tau;

    /// copy all local states from _tau
    auto locals = _tau.get_locals();
    /// do acceleration: set the counter of l as w
    for (const auto& l : wls) {
        locals[l] = refer::omega;
    }
    /// return a new global states
    return global_state(_tau.get_share(), locals, _tau.get_depth());
}

/**
 * @brief update the prefix of path leads to state <tau>. The update
 *        includes the following steps:
 *        step 1. remove all global state whose depth >= tau.depth;
 *        step 2. push tau into sigma that represents a trace leads
 *                to tau, as all states whose discovering time are
 *                later than tau's have already been removed.
 *        NOTE: the backtracking operation has been covered
 * @param tau  : the current state
 * @param sigma: the prefix of path stored in DFS
 */
void GKM::update_predecessors(const global_state& tau,
        deque<global_state>& sigma) {
    auto ip = sigma.begin();
    while (ip != sigma.end()) {
        if (ip->get_depth() >= tau.get_depth())
            break;
        ++ip;
    }
    /// remove all global state whose depth >= tau.depth
    sigma.erase(ip, sigma.end());
    /// push tau into sigma that represents a trace leads to tau
    sigma.emplace_back(tau);
}

/**
 * @brief standard finite state search
 * @return
 */
bool GKM::standard_FWS() {
    size_p n = 1, s = 1;
    while (n < 11) {
        if (this->standard_FWS(n, s)) {
            if (refer::OPT_INPUT_TTS)
                this->standard_FWS(n + 1, s + 1);
            return true;
        }
        ++n, ++s;
    }
    return false;
}

/**
 * @brief this is based on breadth-first search
 * @param n
 * @param s
 * @return true if final state is reachable under n threads and
 */
bool GKM::standard_FWS(const size_p& n, const size_p& s) {
    auto spw = s;
    antichain worklist;
    antichain explored;
    worklist.emplace_back(initl_TS, n);
    while (!worklist.empty()) {
        const auto tau = worklist.front();
        worklist.pop_front();
        /// step 1: if upward(tau) is already explored, then
        ///         discard it
        if (!this->is_maximal(tau, explored))
            continue;

        /// step 2: compute all post images; check if final
        ///         state is coverable; maximize <worklist>
        const auto& images = this->step(tau, spw);
        for (const auto& _tau : images) {
            /// return true if _tau covers final state
            if (this->is_reached(_tau))
                return true;
            /// if upward(_tau) already exists, then discard it
            if (!this->is_maximal(_tau, worklist))
                continue;
            /// maximize <worklist> in term of _tau
            this->maximize(_tau, worklist);
            worklist.emplace_back(_tau); /// insert into worklist
        }
        /// maximize <explored> in term of tau
        this->maximize(tau, explored);
        explored.emplace_back(tau); /// insert into explored
    }
    return false;
}

/**
 * @brief image computation for finite state search
 * @param tau
 * @param spw
 * @return the set of post images
 */
deque<global_state> GKM::step(const global_state& tau, size_p& spw) {
    deque<global_state> images;
    const auto& s = tau.get_share();
    for (const auto& p : tau.get_locals()) {
        const auto& l = p.first;
        const thread_state curr(s, l);
        auto ifind = this->original_TTS.find(curr);
        if (ifind != this->original_TTS.end()) {
            const auto& successors = ifind->second;
            for (const auto& succ : successors) {
                if (this->is_spawn_transition(curr, succ)) {
                    if (spw > 0)
                        spw--;
                    else
                        continue;
                    /// update counters in term of spawn  transition
                    const auto& _Z = this->update_counter(succ.get_local(),
                            tau.get_locals());
                    images.emplace_back(succ.get_share(), _Z);
                } else {
                    /// update counters in term of normal transition
                    const auto& _Z = this->update_counter(l, succ.get_local(),
                            tau.get_locals());
                    images.emplace_back(succ.get_share(), _Z);
                }
            }
        }
    }
    return images;
}

/**
 * @brief check if some already-explored state covers s, and maximize explored
 * @param s
 * @param explored
 * @return
 */
bool GKM::is_maximal(const global_state& s, const antichain& explored) {
    for (auto itau = explored.begin(); itau != explored.end(); ++itau) {
        if (this->is_covered(s, *itau)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief maximize
 * @param worklist
 */
void GKM::maximize(const global_state& s, antichain& worklist) {
    for (auto itau = worklist.begin(); itau != worklist.end();) {
        if (this->is_covered(*itau, s)) {
            itau = worklist.erase(itau);
        } else {
            ++itau;
        }
    }
}

/**
 * @brief determine if (src, dst) corresponds to a spawn transition
 * @param src
 * @param dst
 * @return true : if exists src +> dst
 *         false: otherwise
 */
bool GKM::is_spawn_transition(const thread_state& src,
        const thread_state& dst) {
    if (this->orispawn_TTS.empty()) /// there exists no spawn transition
        return false;

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
 * @brief to determine if s belongs to upward-closure of initial states
 * @param s
 * @return true : if s is in the upward-closure of initial state
 *         false: otherwise
 */
bool GKM::is_reached(const global_state& s) {
    if (s.get_share() == this->final_TS.get_share()) {
        cout << "cover: " << s << "\n";
        auto ifind = s.get_locals().find(final_TS.get_local());
        if (ifind != s.get_locals().cend() && ifind->second > 0)
            return true;
    }
    return false;
}

/**
 * @brief to determine whether s1 is covered by s2.
 *        NOTE: this function assumes that the local parts of s1 and s2
 *        are ordered.
 * @param s1
 * @param s2
 * @return true : if s1 <= s2
 *         false: otherwise
 */
bool GKM::is_covered(const global_state& s1, const global_state& s2) {
    if (s1.get_share() == s2.get_share()
            && s1.get_locals().size() <= s2.get_locals().size()) {
        auto is1 = s1.get_locals().cbegin();
        auto is2 = s2.get_locals().cbegin();
        while (is1 != s1.get_locals().cend()) {
            /// check if is2 reaches to the end
            if (is2 == s2.get_locals().cend())
                return false;
            /// compare the map's contents
            if (is1->first == is2->first) {
                if (is1->second <= is2->second)
                    ++is1, ++is2;
                else
                    return false;
            } else if (is1->first > is2->first) {
                ++is2;
            } else if (is1->first < is2->first) {
                return false;
            }
        }
        return true;
    }
    return false;
}

/**
 * @brief update counters for normal transitions
 * @param s
 * @param l
 * @param _s
 * @param _l
 * @param Z
 * @return global state
 */
global_state GKM::update_counter(const shared_state& s, const local_state& l,
        const shared_state& _s, const local_state& _l, const ca_locals& Z) {
    if (l == _l)
        return global_state(_s, Z);

    auto _Z = Z;

/// step 1: update or eliminate pair for the decremental local state
///         skip update if its counter is w as w - 1 = w
    auto idec = _Z.find(l);
    if (idec != _Z.end() && idec->second != refer::omega) {
        idec->second -= 1;
        if (idec->second == 0) /// remove it if a counter ever becomes zero
            _Z.erase(idec);
    }

/// step 2: update or add pair for the incremental local state
    auto iinc = _Z.find(_l);
    if (iinc != _Z.end()) {
        if (iinc->second != refer::omega) {
            if (idec->second == refer::omega && s == _s) {
                /// add with w if transition can be re-taken
                iinc->second = refer::omega;
            } else {
                /// otherwise, plus one
                iinc->second += 1;
            }
        }
        /// skip update if its counter is w as w + 1 = w
    } else if (idec->second == refer::omega && s == _s) {
        /// add with w if transition can be re-taken
        _Z.emplace(_l, refer::omega);
    } else {
        /// create a new pair if exists no such pair in previous
        _Z.emplace(_l, 1);
    }

    return global_state(_s, _Z);
}

/**
 * @brief update counters for spawn transitions
 * @param s
 * @param _s
 * @param _l
 * @param Z
 * @return global state
 */
global_state GKM::update_counter(const shared_state& s, const shared_state& _s,
        const local_state& _l, const ca_locals& Z) {
    auto _Z = Z;

    auto iinc = _Z.find(_l);
    if (iinc != _Z.end()) {
        if (iinc->second != refer::omega) {
            if (s == _s) {
                iinc->second = refer::omega;
            } else {
                iinc->second += 1;
            }
        }
    } else {
        _Z.emplace(_l, 1);
    }

    return global_state(_s, _Z);
}

/**
 * @brief update counter with w acceleration
 * @param l : local state to decrement
 * @param _l: local state to increment
 * @param Z : counter abstraction representation
 * @return counter abstraction representation after counter updates
 */
ca_locals GKM::update_counter(const local_state& l, const local_state&_l,
        const ca_locals& Z) {
    if (_l == l)
        return Z;

    auto _Z = Z;

    auto idec = _Z.find(l);
    if (idec != _Z.end()) {
        idec->second -= 1;
        if (idec->second == 0)
            _Z.erase(idec);
    }

    auto iinc = _Z.find(_l);
    if (iinc != _Z.end()) {
        iinc->second += 1;
    } else {
        _Z.emplace(_l, 1);
    }

    return _Z;
}

/**
 * @brief update counter with w acceleration
 * @param _l: local state to increment
 * @param Z : counter abstraction representation
 * @return counter abstraction representation after counter updates
 */
ca_locals GKM::update_counter(const local_state&_l, const ca_locals& Z) {
    auto _Z = Z;

    auto iinc = _Z.find(_l);
    if (iinc != _Z.end()) {
        iinc->second += 1;
    } else {
        _Z.emplace(_l, 1);
    }

    return _Z;
}

} /* namespace iotf */
