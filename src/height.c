#include "SPHeight.h"

SPVec4 spHeightGet(SPVec4 previousHeight, //if spReplacesPreviousHeight returns false, then previousHeight is the output of the previous mod, otherwise it should be ignored.
                   SPNoise* noise1,
                   SPNoise* noise2,
                   SPVec3 pointNormal,
                   SPVec3 noiseLoc,
                   SPWorldGenOptions worldGenOptions,
                   double riverValue,
                   double riverDistance) {


    SPVec4 result = {SP_METERS_TO_PRERENDER(spMax(noiseLoc.x, noiseLoc.y)), riverDistance, 0.0, 0.0};
    return result;
}