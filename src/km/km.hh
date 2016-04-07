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
using syst_state = global_state;
using syst_thread = thread_state;
using antichain = deque<syst_state>;

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
    deque<syst_thread> initl_TS;
    deque<syst_thread> final_TS;
    adj_list original_TTS;
    adj_list orispawn_TTS;

    bool standard_GKM();
    deque<syst_state> step(const syst_state& tau);
    syst_state w_acceleration(const syst_state& tau,
            const deque<syst_state>& sigma);
    void update_predecessors(const syst_state& tau, deque<syst_state>& sigms);

    bool onthefly_GKM(const string& filename);

    bool standard_FWS();
    bool standard_FWS(const size_p& n, const size_p& s);
    deque<syst_state> step(const syst_state& tau, size_p& spw);

    bool onthefly_FWS(const size_p& n, const string& filename);

    bool is_spawn_transition(const syst_thread& src, const syst_thread& dst);
    bool is_maximal(const syst_state& s, const antichain& explored);
    void maximize(const syst_state& s, antichain& worklist);

    bool is_reached(const syst_state& s);
    bool is_covered(const syst_state& s1, const syst_state& s2);

    syst_state update_counter(const shared_state& s, const local_state& l,
            const shared_state& _s, const local_state& _l, const ca_locals& Z);
    syst_state update_counter(const shared_state& s, const shared_state& _s,
            const local_state& _l, const ca_locals& Z);
    ca_locals update_counter(const local_state& l, const local_state&_l,
            const ca_locals& Z);
    ca_locals update_counter(const local_state&_l, const ca_locals& Z);

    void parse_TTS(const string& filename,
            const string& s_initl, const string& s_final,
            const bool& is_self_loop);
    syst_thread set_up_TS(const string& s_ts);

    /// some testing functions
    string parse_BP(const string& filename);
};

} /* namespace iotf */

#endif /* KM_KM_HH_ */
