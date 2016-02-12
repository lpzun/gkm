/**************************************************************************************
 *@name             : GKM
 *                        a generalized version of karp-miller procedure
 *@author           : Peizun Liu
 *@version          : 0.1 beta
 *
 *@copyright        : The copy right belongs to Thomas Wahl's group, CCIS, NEU, Boston
 *
 *@brief            : This tool aims to deal with program state coverability problem
 *                    for non-recursive concurrent C programs with any number of threads.
 *************************************************************************************/
#include <iostream>

#include "km/km.hh"
#include "util/cmd.hh"
#include "util/refs.hh"

using namespace gkm;
using namespace std;

int main(const int argc, const char * const * const argv) {
    try {
        cmd_line cmd;
        try {
            cmd.get_command_line(argc, argv);
        } catch (cmd_line::Help) {
            return 0;
        }

        Refs::OPT_PRINT_ADJ = cmd.arg_bool(cmd_line::prob_inst_opts(),
                "--adj-list");
        Refs::OPT_INPUT_TTS = cmd.arg_bool(cmd_line::prob_inst_opts(),
                "--input-tts");

        Refs::OPT_PRINT_CMD = cmd.arg_bool(cmd_line::other_opts(),
                "--cmd-line");
        Refs::OPT_PRINT_ALL = cmd.arg_bool(cmd_line::other_opts(), "--all");

        const string& filename = cmd.arg_value(cmd_line::prob_inst_opts(),
                "--input-file");
        const string& initl_ts = cmd.arg_value(cmd_line::prob_inst_opts(),
                "--initial");
        const string& final_ts = cmd.arg_value(cmd_line::prob_inst_opts(),
                "--target");

        bool is_reachable = false;
        cout << "======================================================\n";
        cout << " (" << final_ts << ")";
        if (is_reachable)
            cout << " is reachable: verification failed!\n";
        else
            cout << " is unreachable: verification successful!\n";
        cout << "======================================================"
                << endl;

    } catch (const bws_exception & e) {
        e.what();
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    } catch (...) {
        std::cerr << bws_exception("main: unknown exception occurred").what()
                << endl;
    }
}




