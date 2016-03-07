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
private:
    /// set omega symbol
    const ushort omega = std::numeric_limits<ushort>::max();

    thread_state initl_TS;
    thread_state final_TS;
    adj_list original_TTS;
    adj_list orispawn_TTS;

    bool standard_GKM();
    deque<global_state> step(const global_state& tau);

    bool is_spawn_transition(const thread_state& src, const thread_state& dst);

    global_state update_counter(const shared_state& s, const local_state& l,
            const shared_state& _s, const local_state& _l, const ca_locals& Z,
            const bool& is_spawn = false);

    global_state w_acceleration(const global_state& tau);

};

} /* namespace iotf */

#endif /* KM_KM_HH_ */
