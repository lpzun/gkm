/**
 * @brief dfs.hh
 *
 * @date  : Mar 9, 2016
 * @author: lpzun
 */

#ifndef KM_DFS_HH_
#define KM_DFS_HH_

#include "../util/heads.hh"

namespace dfs {
class node {
public:
    node(const int& val) :
            val(val), depth(0) {

    }
    node(const int& val, const int& depth) :
            val(val), depth(depth) {

    }

    int get_depth() const {
        return depth;
    }

    void set_depth(int depth) {
        this->depth = depth;
    }

    int get_val() const {
        return val;
    }

    void set_val(int val) {
        this->val = val;
    }

private:
    int val;
    int depth;
};

inline bool operator <(const node& n1, const node& n2) {
    return n1.get_val() < n2.get_val();
}

inline bool operator >(const node& n1, const node& n2) {
    return n2 < n1;
}

inline bool operator ==(const node& n1, const node& n2) {
    return n1.get_val() == n2.get_val();
}

inline bool operator !=(const node& n1, const node& n2) {
    return n1 != n2;
}

inline ostream& operator <<(ostream& out, const node& n) {
    out << n.get_val();// << " at depth = " << n.get_depth();
    return out;
}

using adj_list=map<node, deque<node>>;

class DFS {
public:
    DFS();
    ~DFS();

    void dfs_visit(const node& root, const adj_list& graph);

    bool is_visited(const node& n, const deque<node>& explored);

    void update_prefix(const node& n, deque<node>& prefix);

    void test_dfs();
};

} /* namespace iotf */

#endif /* KM_DFS_HH_ */
