/**
 * @name refs.cc
 *
 * @date: Jun 21, 2015
 * @author: Peizun Liu
 */

#include "refs.hh"

namespace gkm {

Refs::Refs() {

}

Refs::~Refs() {

}
bool Refs::OPT_INPUT_TTS = false;

bool Refs::OPT_PRINT_ADJ = false;
bool Refs::OPT_PRINT_CMD = false;
bool Refs::OPT_PRINT_ALL = false;

clock_t Refs::ELAPSED_TIME = clock();

const ushort Refs::omega = std::numeric_limits<ushort>::max();

} /* namespace SURA */
