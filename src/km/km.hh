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

    thread_state initl_TS;
    thread_state final_TS;
    adj_list original_TTS;
    adj_list orispawn_TTS;

    bool standard_GKM();
    deque<global_state> step(const global_state& tau);

    bool standard_FWS();
    bool standard_FWS(const size_p& n, const size_p& s);
    deque<global_state> step(const global_state& tau, size_p& spw);

    bool is_spawn_transition(const thread_state& src, const thread_state& dst);

    bool is_reached(const global_state& s);
    bool is_covered(const global_state& s1, const global_state& s2);

    global_state update_counter(const shared_state& s, const local_state& l,
            const shared_state& _s, const local_state& _l, const ca_locals& Z);
    global_state update_counter(const shared_state& s, const shared_state& _s,
            const local_state& _l, const ca_locals& Z);

    ca_locals update_counter(const local_state& l, const local_state&_l,
            const ca_locals& Z);
    ca_locals update_counter(const local_state&_l, const ca_locals& Z);

    global_state w_acceleration(const global_state& tau);

    bool is_explored(const global_state& s, const antichain& explored);
    void maximize(const global_state& s, antichain& worklist);

    thread_state set_up_TS(const string& s_ts);
};

} /* namespace iotf */

#endif /* KM_KM_HH_ */
