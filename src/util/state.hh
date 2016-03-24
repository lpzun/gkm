/**
 * @name state.hh
 *
 * @date  : Jun 21, 2015
 * @author: Peizun Liu
 */

#ifndef STATE_HH_
#define STATE_HH_

#include "algs.hh"
#include "refs.hh"

namespace gkm {

/// define size of shared/local states
typedef uint size_s;
/// define local state
typedef size_s local_state;

/// define shared state
typedef uint shared_state;

/// define the counter of thread state
typedef unsigned short size_p;

/// class thread state
class thread_state {
public:
    static size_s S; /// the size of shared state
    static size_s L; /// the size of local  state

    inline thread_state();
    inline thread_state(const thread_state& t);
    inline thread_state(const shared_state& share, const local_state& local);
    ~thread_state() {
    }

    ostream& to_stream(ostream& out = cout) const;

    inline local_state get_local() const {
        return local;
    }

    inline shared_state get_share() const {
        return share;
    }

private:
    shared_state share;
    local_state local;
};

/**
 * @brief default constructor
 */
inline thread_state::thread_state() :
        share(0), local(0) {
}

/**
 * @brief constructor with thread state
 * @param t
 */
inline thread_state::thread_state(const thread_state& t) :
        share(t.share), local(t.local) {

}

/**
 * @brief constructor with shared state and local state
 * @param share: shared state
 * @param local: local  state
 */
inline thread_state::thread_state(const shared_state& share,
        const local_state& local) :
        share(share), local(local) {
    __SAFE_ASSERT__(share < S && local < L);
}

/**
 * @brief print thread state
 * @param out
 * @return ostream
 */
inline ostream& thread_state::to_stream(ostream& out) const {
    out << "(" << share << "|" << local << ")";
    return out;
}

/**
 * @brief overloading operator <<: print thread state
 * @param out
 * @param ts
 * @return ostream
 */
inline ostream& operator<<(ostream& out, const thread_state& t) {
    return t.to_stream(out);
}

/**
 * @brief overloading operator ==
 * @param t1
 * @param t2
 * @return bool
 * 		   true : if t1 == t2
 * 		   false: otherwise
 */
inline bool operator==(const thread_state& t1, const thread_state& t2) {
    return t1.get_share() == t2.get_share() && t1.get_local() == t2.get_local();
}

/**
 * @brief overloading operator !=
 * @param t1
 * @param t2
 * @return bool
 * 		   true : if t1 == t2
 * 		   false: otherwise
 */
inline bool operator!=(const thread_state& t1, const thread_state& t2) {
    return !(t1 == t2);
}

/**
 * @brief overloading operator <
 * @param t1
 * @param t2
 * @return bool
 * 		   true : if t1 < t2
 * 		   false: otherwise
 */
inline bool operator<(const thread_state& t1, const thread_state& t2) {
    if (t1.get_share() == t2.get_share())
        return t1.get_local() < t2.get_local();
    else
        return t1.get_share() < t2.get_share();
}

/**
 * @brief overloading operator >
 * @param t1
 * @param t2
 * @return bool
 * 		   true : if t1 > t2
 * 		   false: otherwise
 */
inline bool operator>(const thread_state& t1, const thread_state& t2) {
    return t2 < t1;
}

using vertex = unsigned int;
/// adjacency list
using adj_list = map<thread_state, deque<thread_state>>;

/// local parts: represent in counter abstraction
using ca_locals = map<local_state, size_p>;

/// class global state
class global_state {
public:
    inline global_state();
    inline global_state(const thread_state& t);
    inline global_state(const thread_state& t, const size_p& n);
    inline global_state(const shared_state& s, const local_state& l,
            const size_p& n);
    inline global_state(const shared_state& share, const ca_locals& locals);
    inline global_state(const global_state& g, const uint& depth);
    inline global_state(const shared_state& share, const ca_locals& locals,
            const uint& depth);

    ~global_state() {
    }

    ostream& to_stream(ostream& out = cout, const string& sep = "|") const;

    inline shared_state get_share() const {
        return share;
    }

    inline const ca_locals& get_locals() const {
        return locals;
    }

    void set_locals(const local_state& l, const size_p& c) {
        this->locals[l] = c;
    }

    uint get_depth() const {
        return depth;
    }

    void set_depth(uint depth) {
        this->depth = depth;
    }

private:
    shared_state share;
    ca_locals locals;
    uint depth;
};

/**
 * @brief default constructor: initialize
 *        share  = 0
 *        locals = empty map
 */
inline global_state::global_state() :
        share(0), locals(ca_locals()), depth(0) {
}

/**
 * @brief constructor with a thread state
 * @param t
 */
inline global_state::global_state(const thread_state& t) :
        share(t.get_share()), locals(ca_locals()), depth(0) {
    locals.emplace(t.get_local(), 1);
}

/**
 * @brief constructor with a thread state and n threads
 * @param t
 * @param n
 */
inline global_state::global_state(const thread_state& t, const size_p& n) :
        share(t.get_share()), locals(ca_locals()), depth(0) {
    locals.emplace(t.get_local(), n);
}

inline global_state::global_state(const shared_state& s, const local_state& l,
        const size_p& n) :
        share(s), locals(ca_locals()), depth(0) {
    locals.emplace(l, n);
}

/**
 * @brief constructor with a shared state and local states
 * @param share : shared state
 * @param locals: local states represented in counter abstraction form
 */
inline global_state::global_state(const shared_state& share,
        const ca_locals& locals) :
        share(share), locals(locals), depth(0) {
}

/**
 * @brief constructor with a shared state, local states and depth
 * @param g    : global state
 * @param depth: the depth of current global state
 */
inline global_state::global_state(const global_state& g, const uint& depth) :
        share(g.get_share()), locals(g.get_locals()), depth(depth) {

}

/**
 * @brief constructor with a shared state, local states and pi
 * @param share : shared state
 * @param locals: local states represented in counter abstraction form
 * @param depth	: the depth of current global state
 */
inline global_state::global_state(const shared_state& share,
        const ca_locals& locals, const uint& depth) :
        share(share), locals(locals), depth(depth) {
}

/**
 * @brief call by <<
 * @param out
 * @param sep
 * @return
 */
inline ostream& global_state::to_stream(ostream& out, const string& sep) const {
    out << "<" << this->share << "|";
    for (auto iloc = this->locals.begin(); iloc != this->locals.end(); ++iloc) {
        out << "(" << iloc->first << ","
                << (iloc->second == refer::omega ?
                        "w" : std::to_string(iloc->second)) << ")";
    }
    out << ">";
    return out;
}

/**
 * @brief overloading operator <<
 * @param out
 * @param g
 * @return
 */
inline ostream& operator<<(ostream& out, const global_state& g) {
    return g.to_stream(out);
}

/**
 * @brief overloading operator <
 * @param s1
 * @param s2
 * @return bool
 * 		   true : if s1 < s2
 * 		   false: otherwise
 */
inline bool operator<(const global_state& s1, const global_state& s2) {
    if (s1.get_share() == s2.get_share()) {
        return COMPARE::compare_map(s1.get_locals(), s2.get_locals()) == -1;
    } else {
        return s1.get_share() < s2.get_share();
    }
}

/**
 * @brief overloading operator >
 * @param s1
 * @param s2
 * @return bool
 * 		   true : if s1 > s2
 * 		   false: otherwise
 */
inline bool operator>(const global_state& s1, const global_state& s2) {
    return s2 < s1;
}

/**
 * @brief overloading operator ==
 * @param s1
 * @param s2
 * @return bool
 * 		   true : if s1 == s2
 * 		   false: otherwise
 */
inline bool operator==(const global_state& s1, const global_state& s2) {
    if (s1.get_share() == s2.get_share()) {
        if (s1.get_locals().size() == s2.get_locals().size()) {
            auto is1 = s1.get_locals().begin(), is2 = s2.get_locals().begin();
            while (is1 != s1.get_locals().end()) {
                if ((is1->first != is2->first) || (is1->second != is2->second))
                    return false;
                is1++, is2++;
            }
            return true;
        }
    }
    return false;
}

/**
 * @brief overloading operator !=
 * @param s1
 * @param s2
 * @return bool
 * 		   true : if s1 != s2
 * 		   false: otherwise
 */
inline bool operator!=(const global_state& s1, const global_state& s2) {
    return !(s1 == s2);
}

} /* namespace SURA */

#endif /* STATE_HH_ */
