/**
 * @brief dfs.cc
 *
 * @date  : Mar 9, 2016
 * @author: lpzun
 */

#include "dfs.hh"

namespace dfs {

DFS::DFS() {
}

DFS::~DFS() {
}

/**
 *
 * @param root
 * @param graph
 */
void DFS::dfs_visit(const node& root, const adj_list& graph) {
    deque<node> worklist;
    deque<node> explored;
    deque<node> prefix;

    worklist.emplace_front(root);
    while (!worklist.empty()) {
        const auto curr = worklist.front();
        worklist.pop_front();

        if (this->is_visited(curr, explored))
            continue;

        this->update_prefix(curr, prefix);

        auto ifind = graph.find(curr);
        if (ifind != graph.end()) {
            for (const auto& succ : ifind->second) {
                if (!is_visited(succ, explored)) {
                    node s(succ.get_val(), curr.get_depth() + 1);
                    worklist.emplace_front(s);
                    cout << s << " prefix = ";
                    for (const auto& p : prefix)
                        cout << p << "->";
                    cout << s << "\n";
                }
            }
        }
        explored.emplace_back(curr);
    }
}

bool DFS::is_visited(const node& n, const deque<node>& explored) {
    for (const auto& v : explored) {
        if (n == v)
            return true;
    }
    return false;
}

void DFS::update_prefix(const node& n, deque<node>& prefix) {
    auto ip = prefix.begin();
    while (ip != prefix.end()) {
        if (ip->get_depth() >= n.get_depth()) {
            prefix.erase(ip, prefix.end());
            break;
        }
    }
    ++ip;
    prefix.emplace_back(n);
}

void DFS::test_dfs() {
    node n0(0);
    node n1(1);
    node n2(2);
    node n3(3);
    node n4(4);

    adj_list graph;

    graph.emplace(n0, deque<node>());
    graph[n0].emplace_back(n2);
    graph[n0].emplace_back(n3);

    graph.emplace(n1, deque<node>());
    graph[n1].emplace_back(n0);

    graph.emplace(n2, deque<node>());
    graph[n2].emplace_back(n1);

    graph.emplace(n3, deque<node>());
    graph[n3].emplace_back(n4);

    graph.emplace(n4, deque<node>());
    graph[n4].emplace_back(n0);

    for (const auto& p : graph) {
        for (const auto& v : p.second)
            cout << p.first.get_val() << "->" << v.get_val() << "\n";
    }

    this->dfs_visit(n0, graph);
}

} /* namespace iotf */
