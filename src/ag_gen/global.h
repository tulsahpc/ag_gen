#ifndef GLOBAL_H
#define GLOBAL_H

class ParameterizedQuality;
class ParameterizedTopology;

using PostconditionQuality = std::tuple<ParameterizedQuality, std::string>;
using PostconditionTopology = std::tuple<ParameterizedTopology, std::string>;

#endif
