/**
 * @brief km.hh
 *
 * @date  : Feb 12, 2016
 * @author: lpzun
 */

#ifndef KM_KM_HH_
#define KM_KM_HH_

#include "../util/state.hh"
#include "../util/algs.hh"
#include "../util/utilities.hh"

#include "../../../iotf/src/iotf.hh"

namespace gkm {

using antichain = deque<global_state>;

class GKM {
public:
    GKM();
    ~GKM();

    bool reachability_analysis_via_gkm(const string& filename,
            const string& initl, const string& final, const bool& is_self_loop =
                    false);

    /// testing methods
    void test_is_covered();
    void test_update_counter();

private:
    deque<thread_state> initl_TS;
    deque<thread_state> final_TS;
    adj_list original_TTS;
    adj_list orispawn_TTS;

    bool standard_GKM();
    bool onthefly_GKM();
    deque<global_state> step(const global_state& tau);
    global_state w_acceleration(const global_state& tau,
            const deque<global_state>& sigma);
    void update_predecessors(const global_state& tau,
            deque<global_state>& sigms);

    bool standard_FWS();
    bool onthefly_FWS(const size_p& n, const string& filename);
    bool standard_FWS(const size_p& n, const size_p& s);
    deque<global_state> step(const global_state& tau, size_p& spw);

    bool is_spawn_transition(const thread_state& src, const thread_state& dst);

    bool is_maximal(const global_state& s, const antichain& explored);
    void maximize(const global_state& s, antichain& worklist);

    bool is_reached(const global_state& s);
    bool is_covered(const global_state& s1, const global_state& s2);

    global_state update_counter(const shared_state& s, const local_state& l,
            const shared_state& _s, const local_state& _l, const ca_locals& Z);
    global_state update_counter(const shared_state& s, const shared_state& _s,
            const local_state& _l, const ca_locals& Z);
    ca_locals update_counter(const local_state& l, const local_state&_l,
            const ca_locals& Z);
    ca_locals update_counter(const local_state&_l, const ca_locals& Z);

    thread_state set_up_TS(const string& s_ts);

    /// some testing functions
    string parse_BP(const string& filename);
};

} /* namespace iotf */

#endif /* KM_KM_HH_ */
