/**
 * @brief test_km.cc
 *
 * @date  : Mar 7, 2016
 * @author: lpzun
 */

#include "km.hh"

namespace gkm {

void GKM::test_is_covered() {
    cout << __func__ << "\n";

    ca_locals z1;
    z1.emplace(1, 2);
    z1.emplace(2, 3);
    z1.emplace(3, 1);

    shared_state s1(1);
    global_state g1(s1, z1);
    cout << g1 << "\n";

    ca_locals z2;
    z2.emplace(1, 3);
    z2.emplace(2, 3);
    z2.emplace(3, 2);

    shared_state s2(1);
    global_state g2(s2, z2);
    cout << g2 << "\n";

    if (is_covered(g1, g2)) {
        cout << "Passed!" << "\n";
    }

    ca_locals z3;
    z3.emplace(1, 1);
    z3.emplace(2, 4);
    z3.emplace(3, 2);
    global_state g3(s1, z3);
    cout << g3 << "\n";
    if (!is_covered(g1, g3)) {
        cout << "Passed!" << "\n";
    }

    ca_locals z4;
    z4.emplace(1, 3);
    z4.emplace(2, 3);
    z4.emplace(3, 2);

    global_state g4(shared_state(2), z4);
    cout << g4 << "\n";

    if (!is_covered(g1, g4)) {
        cout << "Passed!" << "\n";
    }

    ca_locals z5;
    z5.emplace(1, 3);
    z5.emplace(3, 3);
    z5.emplace(4, 2);

    global_state g5(s1, z5);
    cout << g5 << "\n";

    if (!is_covered(g1, g5)) {
        cout << "Passed!" << "\n";
    }

    ca_locals z6;
    z6.emplace(1, 3);
    z6.emplace(2, 3);
    z6.emplace(3, 3);
    z6.emplace(4, refer::omega);

    global_state g6(s1, z6);
    cout << g6 << "\n";

    if (is_covered(g1, g6)) {
        cout << "Passed!" << "\n";
    }


}

void GKM::test_update_counter() {
    ca_locals z1;
    z1.emplace(1, 2);
    z1.emplace(2, 3);
    z1.emplace(3, 1);


}

}

