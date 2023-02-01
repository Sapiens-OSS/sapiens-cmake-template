

#include "SPBiome.h"
#include <string.h>

// WARNING these variables are not thread local, but they should be. So any static storage here created on one thread may be overwritten with values from another thread, which can definitely cause problems.
// typeMaps should generally be consistent between the server, the logic thread, and the main thread when the game is running. However they are not consistent between the globe view thread and the rest, so the globe thread must be finished with before the others are loaded.
// possibly could use __declspec(thread) (MSVC) and __thread (for GNU), though not sure on macOS, maybe c11's thread_local

static uint16_t biomeTag_hot;
static uint16_t biomeTag_wet;
static uint16_t biomeTag_dry;
static uint16_t biomeTag_desert;
static uint16_t biomeTag_steppe;
static uint16_t biomeTag_rainforest;
static uint16_t biomeTag_tropical;
static uint16_t biomeTag_savanna;
static uint16_t biomeTag_polar;
static uint16_t biomeTag_icecap;
static uint16_t biomeTag_tundra;
static uint16_t biomeTag_temperate;
static uint16_t biomeTag_heavySnowWinter;
static uint16_t biomeTag_medSnowWinter;
static uint16_t biomeTag_lightSnowWinter;
static uint16_t biomeTag_heavySnowSummer;
static uint16_t biomeTag_medSnowSummer;
static uint16_t biomeTag_lightSnowSummer;
static uint16_t biomeTag_drySummer;
static uint16_t biomeTag_dryWinter;
static uint16_t biomeTag_verySparseForest;
static uint16_t biomeTag_sparseForest;
static uint16_t biomeTag_mediumForest;
static uint16_t biomeTag_denseForest;
static uint16_t biomeTag_coniferous;
static uint16_t biomeTag_birch;
static uint16_t biomeTag_bamboo;
static uint16_t biomeTag_cliff;
static uint16_t biomeTag_river;


static uint16_t biomeTag_temperatureSummerVeryHot;
static uint16_t biomeTag_temperatureSummerHot;
static uint16_t biomeTag_temperatureSummerModerate;
static uint16_t biomeTag_temperatureSummerCold;
static uint16_t biomeTag_temperatureSummerVeryCold;

static uint16_t biomeTag_temperatureWinterVeryHot;
static uint16_t biomeTag_temperatureWinterHot;
static uint16_t biomeTag_temperatureWinterModerate;
static uint16_t biomeTag_temperatureWinterCold;
static uint16_t biomeTag_temperatureWinterVeryCold;

static uint32_t terrainBaseType_rock;
static uint32_t terrainBaseType_limestone;
static uint32_t terrainBaseType_beachSand;
static uint32_t terrainBaseType_riverSand;
static uint32_t terrainBaseType_desertSand;
static uint32_t terrainBaseType_ice;
static uint32_t terrainBaseType_desertRedSand;
static uint32_t terrainBaseType_redRock;
static uint32_t terrainBaseType_greenRock;
static uint32_t terrainBaseType_dirt;
static uint32_t terrainBaseType_richDirt;
static uint32_t terrainBaseType_poorDirt;
static uint32_t terrainBaseType_clay;

static uint32_t terrainVariation_snow;
static uint32_t terrainVariation_grassSnow;
static uint32_t terrainVariation_temperateGrass;
static uint32_t terrainVariation_temperateGrassPlentiful;
static uint32_t terrainVariation_temperateGrassWinter;
static uint32_t terrainVariation_taigaGrass;
static uint32_t terrainVariation_mediterraneanGrass;
static uint32_t terrainVariation_mediterraneanGrassPlentiful;
static uint32_t terrainVariation_steppeGrass;
static uint32_t terrainVariation_tropicalRainforestGrass;
static uint32_t terrainVariation_tropicalRainforestGrassPlentiful;
static uint32_t terrainVariation_savannaGrass;
static uint32_t terrainVariation_tundraGrass;
static uint32_t terrainVariation_limestone;
static uint32_t terrainVariation_redRock;
static uint32_t terrainVariation_greenRock;
static uint32_t terrainVariation_flint;
static uint32_t terrainVariation_clay;

static uint32_t terrainVariation_shallowWater;
static uint32_t terrainVariation_deepWater;



static uint32_t terrainModifcation_snowRemoved;
static uint32_t terrainModifcation_vegetationRemoved;
static uint32_t terrainModifcation_preventGrassAndSnow;

static uint32_t gameObjectType_appleTree;
static uint32_t gameObjectType_orangeTree;
static uint32_t gameObjectType_peachTree;
static uint32_t gameObjectType_elderberryTree;
static uint32_t gameObjectType_tallPine;
static uint32_t gameObjectType_pineBig1;
static uint32_t gameObjectType_aspenBig1;
//static uint32_t gameObjectType_aspen3;

static uint32_t gameObjectType_bananaTree;
static uint32_t gameObjectType_coconutTree;

static uint32_t gameObjectType_sunflower;
static uint32_t gameObjectType_raspberryBush;
static uint32_t gameObjectType_gooseberryBush;
static uint32_t gameObjectType_beetrootPlant;
static uint32_t gameObjectType_wheatPlant;
static uint32_t gameObjectType_flaxPlant;
static uint32_t gameObjectType_pumpkinPlant;
static uint32_t gameObjectType_poppyPlant;
static uint32_t gameObjectType_echinaceaPlant;
static uint32_t gameObjectType_gingerPlant;
static uint32_t gameObjectType_turmericPlant;
static uint32_t gameObjectType_marigoldPlant;
static uint32_t gameObjectType_garlicPlant;
static uint32_t gameObjectType_aloePlant;

static uint32_t gameObjectType_rock;
static uint32_t gameObjectType_rockSmall;
static uint32_t gameObjectType_rockLarge;
static uint32_t gameObjectType_redRock;
static uint32_t gameObjectType_redRockSmall;
static uint32_t gameObjectType_redRockLarge;
static uint32_t gameObjectType_greenRock;
static uint32_t gameObjectType_greenRockSmall;
static uint32_t gameObjectType_greenRockLarge;
static uint32_t gameObjectType_limestoneRock;
static uint32_t gameObjectType_limestoneRockSmall;
static uint32_t gameObjectType_limestoneRockLarge;
static uint32_t gameObjectType_flint;
static uint32_t gameObjectType_clay;

static uint32_t gameObjectType_birchBranch;
static uint32_t gameObjectType_pineBranch;
static uint32_t gameObjectType_willowBranch;
static uint32_t gameObjectType_bambooBranch;

static uint32_t gameObjectType_deadAlpaca;
static uint32_t gameObjectType_deadMammoth;
static uint32_t gameObjectType_bone;

#define BIRCH_TYPE_COUNT 4
static uint32_t gameObjectType_birchTypes[BIRCH_TYPE_COUNT];

#define ASPEN_TYPE_COUNT 3
static uint32_t gameObjectType_aspenTypes[ASPEN_TYPE_COUNT];

#define MED_PINE_TYPE_COUNT 2
static uint32_t gameObjectType_medPineTypes[MED_PINE_TYPE_COUNT];

#define WILLOW_TYPE_COUNT 2
static uint32_t gameObjectType_willowTypes[WILLOW_TYPE_COUNT];

#define BAMBOO_TYPE_COUNT 1
static uint32_t gameObjectType_bambooTypes[BAMBOO_TYPE_COUNT];

static uint32_t gameObjectType_smallPine;
static uint32_t gameObjectType_smallBamboo;

void spBiomeInit(SPBiomeThreadState* threadState)
{
	biomeTag_hot = threadState->getBiomeTag(threadState, "hot");
	biomeTag_wet = threadState->getBiomeTag(threadState, "wet");
	biomeTag_dry = threadState->getBiomeTag(threadState, "dry");
	biomeTag_desert = threadState->getBiomeTag(threadState, "desert");
	biomeTag_steppe = threadState->getBiomeTag(threadState, "steppe");
	biomeTag_rainforest = threadState->getBiomeTag(threadState, "rainforest");
	biomeTag_tropical = threadState->getBiomeTag(threadState, "tropical");
	biomeTag_savanna = threadState->getBiomeTag(threadState, "savanna");
	biomeTag_polar = threadState->getBiomeTag(threadState, "polar");
	biomeTag_icecap = threadState->getBiomeTag(threadState, "icecap");
	biomeTag_tundra = threadState->getBiomeTag(threadState, "tundra");
	biomeTag_temperate = threadState->getBiomeTag(threadState, "temperate");
	biomeTag_heavySnowWinter = threadState->getBiomeTag(threadState, "heavySnowWinter");
	biomeTag_medSnowWinter = threadState->getBiomeTag(threadState, "medSnowWinter");
	biomeTag_lightSnowWinter = threadState->getBiomeTag(threadState, "lightSnowWinter");
	biomeTag_heavySnowSummer = threadState->getBiomeTag(threadState, "heavySnowSummer");
	biomeTag_medSnowSummer = threadState->getBiomeTag(threadState, "medSnowSummer");
	biomeTag_lightSnowSummer = threadState->getBiomeTag(threadState, "lightSnowSummer");
	biomeTag_drySummer = threadState->getBiomeTag(threadState, "drySummer");
	biomeTag_dryWinter = threadState->getBiomeTag(threadState, "dryWinter");
	biomeTag_verySparseForest = threadState->getBiomeTag(threadState, "verySparseForest");
	biomeTag_sparseForest = threadState->getBiomeTag(threadState, "sparseForest");
	biomeTag_mediumForest = threadState->getBiomeTag(threadState, "mediumForest");
	biomeTag_denseForest = threadState->getBiomeTag(threadState, "denseForest");
	biomeTag_coniferous = threadState->getBiomeTag(threadState, "coniferous");
	biomeTag_birch = threadState->getBiomeTag(threadState, "birch");
	biomeTag_cliff = threadState->getBiomeTag(threadState, "cliff");
	biomeTag_river = threadState->getBiomeTag(threadState, "river");
	biomeTag_bamboo = threadState->getBiomeTag(threadState, "bamboo");

	biomeTag_temperatureSummerVeryHot =		threadState->getBiomeTag(threadState, "temperatureSummerVeryHot");
	biomeTag_temperatureSummerHot =			threadState->getBiomeTag(threadState, "temperatureSummerHot");
	biomeTag_temperatureSummerModerate =	threadState->getBiomeTag(threadState, "temperatureSummerModerate");
	biomeTag_temperatureSummerCold =		threadState->getBiomeTag(threadState, "temperatureSummerCold");
	biomeTag_temperatureSummerVeryCold =	threadState->getBiomeTag(threadState, "temperatureSummerVeryCold");

	biomeTag_temperatureWinterVeryHot =		threadState->getBiomeTag(threadState, "temperatureWinterVeryHot");
	biomeTag_temperatureWinterHot =			threadState->getBiomeTag(threadState, "temperatureWinterHot");
	biomeTag_temperatureWinterModerate =	threadState->getBiomeTag(threadState, "temperatureWinterModerate");
	biomeTag_temperatureWinterCold =		threadState->getBiomeTag(threadState, "temperatureWinterCold");
	biomeTag_temperatureWinterVeryCold =	threadState->getBiomeTag(threadState, "temperatureWinterVeryCold");

	terrainBaseType_rock									= threadState->getTerrainBaseTypeIndex(threadState, "rock");
	terrainBaseType_limestone								= threadState->getTerrainBaseTypeIndex(threadState, "limestone");
	terrainBaseType_beachSand								= threadState->getTerrainBaseTypeIndex(threadState, "beachSand");
	terrainBaseType_riverSand								= threadState->getTerrainBaseTypeIndex(threadState, "riverSand");
	terrainBaseType_desertSand								= threadState->getTerrainBaseTypeIndex(threadState, "desertSand");
	terrainBaseType_ice										= threadState->getTerrainBaseTypeIndex(threadState, "ice");
	terrainBaseType_desertRedSand							= threadState->getTerrainBaseTypeIndex(threadState, "desertRedSand");
	terrainBaseType_redRock									= threadState->getTerrainBaseTypeIndex(threadState, "redRock");
	terrainBaseType_greenRock								= threadState->getTerrainBaseTypeIndex(threadState, "greenRock");
	terrainBaseType_dirt									= threadState->getTerrainBaseTypeIndex(threadState, "dirt");
	terrainBaseType_richDirt								= threadState->getTerrainBaseTypeIndex(threadState, "richDirt");
	terrainBaseType_poorDirt								= threadState->getTerrainBaseTypeIndex(threadState, "poorDirt");
	terrainBaseType_clay									= threadState->getTerrainBaseTypeIndex(threadState, "clay");
															
	terrainVariation_snow									= threadState->getTerrainVariation(threadState, "snow");
	terrainVariation_grassSnow								= threadState->getTerrainVariation(threadState, "grassSnow");
	terrainVariation_temperateGrass							= threadState->getTerrainVariation(threadState, "temperateGrass");
	terrainVariation_temperateGrassPlentiful				= threadState->getTerrainVariation(threadState, "temperateGrassPlentiful");
	terrainVariation_temperateGrassWinter					= threadState->getTerrainVariation(threadState, "temperateGrassWinter");
	terrainVariation_taigaGrass								= threadState->getTerrainVariation(threadState, "taigaGrass");
	terrainVariation_mediterraneanGrass						= threadState->getTerrainVariation(threadState, "mediterraneanGrass");
	terrainVariation_mediterraneanGrassPlentiful			= threadState->getTerrainVariation(threadState, "mediterraneanGrassPlentiful");	
	terrainVariation_steppeGrass							= threadState->getTerrainVariation(threadState, "steppeGrass");
	terrainVariation_tropicalRainforestGrass				= threadState->getTerrainVariation(threadState, "tropicalRainforestGrass");
	terrainVariation_tropicalRainforestGrassPlentiful		= threadState->getTerrainVariation(threadState, "tropicalRainforestGrassPlentiful");
	terrainVariation_savannaGrass							= threadState->getTerrainVariation(threadState, "savannaGrass");
	terrainVariation_tundraGrass							= threadState->getTerrainVariation(threadState, "tundraGrass");
	terrainVariation_flint									= threadState->getTerrainVariation(threadState, "flint");
	terrainVariation_clay									= threadState->getTerrainVariation(threadState, "clay");
	terrainVariation_limestone								= threadState->getTerrainVariation(threadState, "limestone");
	terrainVariation_redRock								= threadState->getTerrainVariation(threadState, "redRock");
	terrainVariation_greenRock								= threadState->getTerrainVariation(threadState, "greenRock");

	terrainVariation_shallowWater							= threadState->getTerrainVariation(threadState, "shallowWater");
	terrainVariation_deepWater								= threadState->getTerrainVariation(threadState, "deepWater");

	terrainModifcation_snowRemoved							= threadState->getTerrainModification(threadState, "snowRemoved");
	terrainModifcation_vegetationRemoved					= threadState->getTerrainModification(threadState, "vegetationRemoved");
	terrainModifcation_preventGrassAndSnow					= threadState->getTerrainModification(threadState, "preventGrassAndSnow");

	if(threadState->getGameObjectTypeIndex) //this function isn't set where game object types aren't required eg. in the initial world creation screen
	{
		gameObjectType_appleTree = threadState->getGameObjectTypeIndex(threadState, "appleTree");
		gameObjectType_orangeTree = threadState->getGameObjectTypeIndex(threadState, "orangeTree");
		gameObjectType_peachTree = threadState->getGameObjectTypeIndex(threadState, "peachTree");
		gameObjectType_elderberryTree = threadState->getGameObjectTypeIndex(threadState, "elderberryTree");
		//gameObjectType_aspen3 = threadState->getGameObjectTypeIndex(threadState, "aspen3");
		gameObjectType_bananaTree = threadState->getGameObjectTypeIndex(threadState, "bananaTree");
		gameObjectType_coconutTree = threadState->getGameObjectTypeIndex(threadState, "coconutTree");

		gameObjectType_sunflower = threadState->getGameObjectTypeIndex(threadState, "sunflower");
		gameObjectType_raspberryBush = threadState->getGameObjectTypeIndex(threadState, "raspberryBush");
		gameObjectType_gooseberryBush = threadState->getGameObjectTypeIndex(threadState, "gooseberryBush");
		gameObjectType_beetrootPlant = threadState->getGameObjectTypeIndex(threadState, "beetrootPlant");
		gameObjectType_pumpkinPlant = threadState->getGameObjectTypeIndex(threadState, "pumpkinPlant");
		gameObjectType_wheatPlant = threadState->getGameObjectTypeIndex(threadState, "wheatPlant");
		gameObjectType_flaxPlant = threadState->getGameObjectTypeIndex(threadState, "flaxPlant");
		gameObjectType_poppyPlant = threadState->getGameObjectTypeIndex(threadState, "poppyPlant");
		gameObjectType_aloePlant = threadState->getGameObjectTypeIndex(threadState, "aloePlant");
		gameObjectType_echinaceaPlant = threadState->getGameObjectTypeIndex(threadState, "echinaceaPlant");
		gameObjectType_marigoldPlant = threadState->getGameObjectTypeIndex(threadState, "marigoldPlant");
		gameObjectType_gingerPlant = threadState->getGameObjectTypeIndex(threadState, "gingerPlant");
		gameObjectType_turmericPlant = threadState->getGameObjectTypeIndex(threadState, "turmericPlant");
		gameObjectType_garlicPlant = threadState->getGameObjectTypeIndex(threadState, "garlicPlant");

		gameObjectType_rock = threadState->getGameObjectTypeIndex(threadState, "rock");
		gameObjectType_rockSmall = threadState->getGameObjectTypeIndex(threadState, "rockSmall");
		gameObjectType_rockLarge = threadState->getGameObjectTypeIndex(threadState, "rockLarge");

		gameObjectType_redRock = threadState->getGameObjectTypeIndex(threadState, "redRock");
		gameObjectType_redRockSmall = threadState->getGameObjectTypeIndex(threadState, "redRockSmall");
		gameObjectType_redRockLarge = threadState->getGameObjectTypeIndex(threadState, "redRockLarge");

		gameObjectType_greenRock = threadState->getGameObjectTypeIndex(threadState, "greenRock");
		gameObjectType_greenRockSmall = threadState->getGameObjectTypeIndex(threadState, "greenRockSmall");
		gameObjectType_greenRockLarge = threadState->getGameObjectTypeIndex(threadState, "greenRockLarge");

		gameObjectType_limestoneRock = threadState->getGameObjectTypeIndex(threadState, "limestoneRock");
		gameObjectType_limestoneRockSmall = threadState->getGameObjectTypeIndex(threadState, "limestoneRockSmall");
		gameObjectType_limestoneRockLarge = threadState->getGameObjectTypeIndex(threadState, "limestoneRockLarge");

		gameObjectType_flint = threadState->getGameObjectTypeIndex(threadState, "flint");
		gameObjectType_birchBranch = threadState->getGameObjectTypeIndex(threadState, "birchBranch");
		gameObjectType_pineBranch = threadState->getGameObjectTypeIndex(threadState, "pineBranch");
		gameObjectType_willowBranch = threadState->getGameObjectTypeIndex(threadState, "willowBranch");
		gameObjectType_bambooBranch = threadState->getGameObjectTypeIndex(threadState, "bambooBranch");
		gameObjectType_clay = threadState->getGameObjectTypeIndex(threadState, "clay");

		gameObjectType_birchTypes[0] = threadState->getGameObjectTypeIndex(threadState, "birch1");
		gameObjectType_birchTypes[1] = threadState->getGameObjectTypeIndex(threadState, "birch2");
		gameObjectType_birchTypes[2] = threadState->getGameObjectTypeIndex(threadState, "birch3");
		gameObjectType_birchTypes[3] = threadState->getGameObjectTypeIndex(threadState, "birch4");

		gameObjectType_aspenTypes[0] = threadState->getGameObjectTypeIndex(threadState, "aspen1");
		gameObjectType_aspenTypes[1] = threadState->getGameObjectTypeIndex(threadState, "aspen2");
		gameObjectType_aspenTypes[2] = threadState->getGameObjectTypeIndex(threadState, "aspen3");

		gameObjectType_willowTypes[0] = threadState->getGameObjectTypeIndex(threadState, "willow1");
		gameObjectType_willowTypes[1] = threadState->getGameObjectTypeIndex(threadState, "willow2");

		gameObjectType_medPineTypes[0] = threadState->getGameObjectTypeIndex(threadState, "pine1");
		gameObjectType_medPineTypes[1] = threadState->getGameObjectTypeIndex(threadState, "pine3");
		gameObjectType_tallPine = threadState->getGameObjectTypeIndex(threadState, "pine2");
		gameObjectType_smallPine = threadState->getGameObjectTypeIndex(threadState, "pine4");

		gameObjectType_pineBig1 = threadState->getGameObjectTypeIndex(threadState, "pineBig1");
		gameObjectType_aspenBig1 = threadState->getGameObjectTypeIndex(threadState, "aspenBig1");

		gameObjectType_bambooTypes[0] = threadState->getGameObjectTypeIndex(threadState, "bamboo1");
		gameObjectType_smallBamboo = threadState->getGameObjectTypeIndex(threadState, "bamboo2");

		gameObjectType_deadAlpaca = threadState->getGameObjectTypeIndex(threadState, "deadAlpaca");
		gameObjectType_deadMammoth = threadState->getGameObjectTypeIndex(threadState, "deadMammoth");
		gameObjectType_bone = threadState->getGameObjectTypeIndex(threadState, "bone");
	}
}

double getSoilRichnessNoiseValue(SPBiomeThreadState* threadState, SPVec3 noiseLoc, double steepness, double riverDistance)
{
	SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 20000.0);
	return spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 4) - steepness * 0.5 + (1.0 - pow(riverDistance, 0.3)) * 0.6;
}

#define VERY_COLD_MAX 0.0f
#define COLD_MAX 10.0f
#define VERY_HOT_MIN 33.0f
#define HOT_MIN 26.0f

void spBiomeGetTagsForPoint(SPBiomeThreadState* threadState,
	uint16_t* tagsOut,
	int* tagCountOut,
	SPVec3 pointNormal,
	SPVec3 noiseLoc,
	double altitude,
	double steepness,
	double riverDistance,
	double temperatureSummer,
	double temperatureWinter,
	double rainfallSummer,
	double rainfallWinter)
{
	int tagCount = *tagCountOut;

	double annualRainfall = rainfallSummer + rainfallWinter;
	double averageTemp = (temperatureSummer + temperatureWinter) * 0.5f;
	double temperatureThreshold = averageTemp * 20.0f;

	SPVec3 scaledNoiseLocC = spVec3Mul(noiseLoc, 802.0);
	double noiseValueC = spNoiseGet(threadState->spNoise1, scaledNoiseLocC, 2);
	//double riverRainfallAddition = (1.0 - pow(riverDistance, 0.05));
	//riverRainfallAddition = riverRainfallAddition * (0.5 + noiseValueC) * 500.0;
	double riverRainfallAddition = (1.0 - pow(spMax(riverDistance - 0.01, 0.0), 0.1));
	riverRainfallAddition = riverRainfallAddition * (1.0 + noiseValueC) * 500.0;

	double annualRainfallWithRiverAddition = annualRainfall + riverRainfallAddition;

	double mixFraction = (rainfallSummer - rainfallWinter * 2.3f) * 0.00001;
	temperatureThreshold = spMix(temperatureThreshold, temperatureThreshold + 200.0f,  mixFraction);


	SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 45999.0);
	double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);

	SPVec3 scaledNoiseLocLargeScale = spVec3Mul(noiseLoc, 8073.0);
	double noiseValueLarge = spNoiseGet(threadState->spNoise1, scaledNoiseLocLargeScale, 2);

	bool isBeach = ((altitude + noiseValue * 0.00000005 + noiseValueLarge * 0.0000005) < 0.0000001);


	if(temperatureWinter < COLD_MAX)
	{
		if(temperatureWinter < VERY_COLD_MAX)
		{
			tagsOut[tagCount++] = biomeTag_temperatureWinterVeryCold;
		}
		else
		{
			tagsOut[tagCount++] = biomeTag_temperatureWinterCold;
		}

		if(annualRainfall > temperatureThreshold * 2.0)
		{
			tagsOut[tagCount++] = biomeTag_heavySnowWinter;
		}
		else if(annualRainfall > temperatureThreshold)
		{
			tagsOut[tagCount++] = biomeTag_medSnowWinter;
		}
		else
		{
			tagsOut[tagCount++] = biomeTag_lightSnowWinter;
		}
	}
	else if (temperatureWinter > HOT_MIN)
	{
		if(temperatureWinter > VERY_HOT_MIN)
		{
			tagsOut[tagCount++] = biomeTag_temperatureWinterVeryHot;
		}
		else
		{
			tagsOut[tagCount++] = biomeTag_temperatureWinterHot;
		}
	}
	else
	{
		tagsOut[tagCount++] = biomeTag_temperatureWinterModerate;
	}


	if(temperatureSummer < COLD_MAX)
	{
		if(temperatureSummer < VERY_COLD_MAX)
		{
			tagsOut[tagCount++] = biomeTag_temperatureSummerVeryCold;
		}
		else
		{
			tagsOut[tagCount++] = biomeTag_temperatureSummerCold;
		}

		if(annualRainfall > temperatureThreshold * 0.5)
		{
			if(annualRainfall > temperatureThreshold * 2.0)
			{
				tagsOut[tagCount++] = biomeTag_heavySnowSummer;
			}
			else if(annualRainfall > temperatureThreshold)
			{
				tagsOut[tagCount++] = biomeTag_medSnowSummer;
			}
			else
			{
				tagsOut[tagCount++] = biomeTag_lightSnowSummer;
			}
		}
	}
	else if (temperatureSummer > HOT_MIN)
	{
		if(temperatureSummer > VERY_HOT_MIN)
		{
			tagsOut[tagCount++] = biomeTag_temperatureSummerVeryHot;
		}
		else
		{
			tagsOut[tagCount++] = biomeTag_temperatureSummerHot;
		}
	}
	else
	{
		tagsOut[tagCount++] = biomeTag_temperatureSummerModerate;
	}

	bool cliff = false;
	if(steepness > 2.0)
	{
		cliff = true;
		tagsOut[tagCount++] = biomeTag_cliff;
	}


	if(riverDistance < 0.01 && altitude < 0.0000002)
	{
		tagsOut[tagCount++] = biomeTag_river;
	}

	if(averageTemp >= 18.0f)
	{
		tagsOut[tagCount++] = biomeTag_hot;
	}

	if (annualRainfallWithRiverAddition < temperatureThreshold) // B
	{
		tagsOut[tagCount++] = biomeTag_dry;

		if (annualRainfallWithRiverAddition < temperatureThreshold * 0.5)
		{
			tagsOut[tagCount++] = biomeTag_desert;
		}
		else
		{
			tagsOut[tagCount++] = biomeTag_steppe;

			if(averageTemp < 18.0f && !cliff && !isBeach)
			{
				SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 8000.0);
				double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
				if(noiseValue > 0.1)
				{
					tagsOut[tagCount++] = biomeTag_verySparseForest;
					tagsOut[tagCount++] = biomeTag_coniferous;
				}
			}
		}

	}
	else
	{
		if (temperatureWinter > 18.0f) // A
		{
			tagsOut[tagCount++] = biomeTag_tropical;

			double driestMonth = spMin(rainfallSummer, rainfallWinter) / 6.0f;
			double treeDensityOffset = 0.0;
			if (driestMonth > 60.0f)
			{
				tagsOut[tagCount++] = biomeTag_rainforest;
				treeDensityOffset = 0.4;
			}
			else
			{
				tagsOut[tagCount++] = biomeTag_savanna;
				treeDensityOffset = - 0.4;
			}

			if(!cliff && !isBeach)
			{
				double noiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance) + treeDensityOffset;
				if(noiseValue > -0.2)
				{
					if(noiseValue > 0.7)
					{
						tagsOut[tagCount++] = biomeTag_denseForest;
					}
					else if(noiseValue > 0.4)
					{
						tagsOut[tagCount++] = biomeTag_mediumForest;
					}
					else
					{
						tagsOut[tagCount++] = biomeTag_sparseForest;
					}
				}
				else
				{
					tagsOut[tagCount++] = biomeTag_verySparseForest;
				}
				tagsOut[tagCount++] = biomeTag_coniferous;
				tagsOut[tagCount++] = biomeTag_bamboo;
			}
		}
		else
		{
			if (temperatureSummer < -2.0f)// EF
			{
				tagsOut[tagCount++] = biomeTag_polar;
				tagsOut[tagCount++] = biomeTag_icecap;
			}
			else if (temperatureSummer < 8.0f)// ET
			{
				tagsOut[tagCount++] = biomeTag_polar;
				tagsOut[tagCount++] = biomeTag_tundra;

				if(!cliff && !isBeach)
				{
					double noiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance);
					if(noiseValue > 0.0)
					{
						if(noiseValue > 0.3)
						{
							tagsOut[tagCount++] = biomeTag_sparseForest;
						}
						else
						{
							tagsOut[tagCount++] = biomeTag_verySparseForest;
						}
						tagsOut[tagCount++] = biomeTag_coniferous;
					}
				}
			}
			else // C or D
			{
				tagsOut[tagCount++] = biomeTag_temperate;

				bool drySummer = false;
				if (rainfallSummer < 30.0f && rainfallSummer < rainfallWinter / 3.0f)
				{
					tagsOut[tagCount++] = biomeTag_drySummer;
					drySummer = true;
				}
				else if (rainfallWinter < rainfallSummer / 10.0f)
				{
					tagsOut[tagCount++] = biomeTag_dryWinter;
				}

				if(!cliff && !isBeach)
				{
					double noiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance);
					if(noiseValue > -0.2)
					{
						if(noiseValue > 0.7)
						{
							tagsOut[tagCount++] = biomeTag_denseForest;
						}
						else if(noiseValue > 0.4)
						{
							tagsOut[tagCount++] = biomeTag_mediumForest;
						}
						else
						{
							tagsOut[tagCount++] = biomeTag_sparseForest;
						}
					}
					else
					{
						tagsOut[tagCount++] = biomeTag_verySparseForest;
					}


					SPVec3 scaledNoiseLocB = spVec3Mul(noiseLoc, 60501.0);
					double noiseValueB = spNoiseGet(threadState->spNoise1, scaledNoiseLocB, 2);
					if(temperatureSummer > noiseValueB * 4.0 + 2.0f)
					{
						if(!drySummer)
						{
							tagsOut[tagCount++] = biomeTag_birch;
						}
						if(temperatureWinter < noiseValueB * 10.0 - 5.0f)
						{
							tagsOut[tagCount++] = biomeTag_coniferous;
						}
					}
					else
					{
						tagsOut[tagCount++] = biomeTag_coniferous;
					}
				}

			}
		}
	}

	*tagCountOut = tagCount;
}

#define rockSteepness 2.0
#define claySteepness 1.6


typedef struct SurfaceTypeInfo {
	bool river;
	bool hot;
	int snowDepth;
} SurfaceTypeInfo;

void getSurfaceTypeInfo(uint16_t* biomeTags, int tagCount, int seasonIndex, SurfaceTypeInfo* surfaceTypeInfo, double steepness, double noiseValue)
{
	for(int i = 0; i < tagCount; i++)
	{
		if(biomeTags[i] == biomeTag_river)
		{
			surfaceTypeInfo->river = true;
		}
		else if(biomeTags[i] == biomeTag_hot)
		{
			surfaceTypeInfo->hot = true;
		}
		else if(biomeTags[i] == biomeTag_heavySnowSummer)
		{
			surfaceTypeInfo->snowDepth = 3;
		}
		else if(biomeTags[i] == biomeTag_medSnowSummer)
		{
			if(seasonIndex == 1)
			{
				surfaceTypeInfo->snowDepth = 2;
			}
			else
			{
				surfaceTypeInfo->snowDepth = 3;
			}
		}
		else if(biomeTags[i] == biomeTag_lightSnowSummer)
		{
			if(seasonIndex == 1)
			{
				surfaceTypeInfo->snowDepth = 1;
			}
			else if(seasonIndex == 0 || seasonIndex == 2)
			{
				surfaceTypeInfo->snowDepth = 2;
			}
			else
			{
				surfaceTypeInfo->snowDepth = 3;
			}
		}
		else if(biomeTags[i] == biomeTag_heavySnowWinter)
		{
			if(seasonIndex == 3)
			{
				surfaceTypeInfo->snowDepth = 3;
			}
		}
		else if(biomeTags[i] == biomeTag_medSnowWinter)
		{
			if(seasonIndex == 3)
			{
				surfaceTypeInfo->snowDepth = 2;
			}
		}
		else if(biomeTags[i] == biomeTag_lightSnowWinter)
		{
			if(seasonIndex == 3)
			{
				surfaceTypeInfo->snowDepth = 1;
			}
		}
	}

	if(surfaceTypeInfo->snowDepth > 0)
	{
		if(steepness > 0.4 + noiseValue * 0.2)
		{
			surfaceTypeInfo->snowDepth--;
		}
		if(steepness > 0.7 + noiseValue * 0.2)
		{
			surfaceTypeInfo->snowDepth--;
		}
	}
}

uint32_t getBeachSurfaceType(SurfaceTypeInfo* surfaceTypeInfo, double riverDistance, double noiseValue, int16_t digFillOffset)
{
	if(surfaceTypeInfo->river)
	{
		if(riverDistance < (noiseValue * 0.01 + 0.005) - (0.04 * digFillOffset))
		{
			return terrainBaseType_riverSand;
		}
		else
		{
			return terrainBaseType_beachSand;
		}
	}

	return (noiseValue > 0.1 + (0.4 * digFillOffset) ? terrainBaseType_riverSand : terrainBaseType_beachSand);
}

static const double SEA_LEVEL = SP_METERS_TO_PRERENDER(-0.1);
static const double DEEP_SEA_LEVEL = SP_METERS_TO_PRERENDER(-1.1);


#define getIsLimestone() (noiseValueMed > 0.2 && noiseValue < 0.2 + noiseValueSmall * 0.5)
#define getIsRedRock() (noiseValueMed < -0.2 && noiseValue > 0.2 && noiseValueSmall > 0.2)
#define getIsGreenRock() (noiseValueMed < -0.3 && noiseValue < -0.2 && noiseValueSmall > 0.2)

SPSurfaceTypeResult spBiomeGetSurfaceTypeForPoint(SPBiomeThreadState* threadState,
	SPSurfaceTypeResult incomingType,
	uint16_t* tags,
	int tagCount,
	uint32_t* modifications,
	int modificationCount,
	uint32_t fillGameObjectTypeIndex,
	int16_t digFillOffset,
	uint32_t* variations,
	SPVec3 pointNormal, 
	SPVec3 noiseLoc, 
	double baseAltitude,
	double steepness,
	double riverDistance,
	int seasonIndex)
{
	SurfaceTypeInfo surfaceTypeInfo;
	SPSurfaceTypeResult result = incomingType;
	memset(&surfaceTypeInfo, 0, sizeof(surfaceTypeInfo));
	SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 45999.0);
	double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);

	SPVec3 scaledNoiseLocSmallScale = spVec3Mul(noiseLoc, 834567.0);
	double noiseValueSmall = spNoiseGet(threadState->spNoise1, scaledNoiseLocSmallScale, 2);

	getSurfaceTypeInfo(tags, tagCount, seasonIndex, &surfaceTypeInfo, steepness, noiseValue);



	SPVec3 scaledNoiseMedScale = spVec3Mul(noiseLoc, 92273.0);
	double noiseValueMed = spNoiseGet(threadState->spNoise1, scaledNoiseMedScale, 2);

	double soilRichnessNoiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance);

	SPVec3 scaledNoiseLocLargeScale = spVec3Mul(noiseLoc, 8073.0);
	double noiseValueLarge = spNoiseGet(threadState->spNoise1, scaledNoiseLocLargeScale, 2);

	uint32_t fillSurfaceBaseType = 0;
	if(fillGameObjectTypeIndex != 0)
	{
		fillSurfaceBaseType = threadState->getSurfaceBaseTypeForFillObjectType(threadState, fillGameObjectTypeIndex);
	}


	/*if(baseAltitude + SP_METERS_TO_PRERENDER((double)digFillOffset) < SEA_LEVEL)
	{
		if(baseAltitude < DEEP_SEA_LEVEL)
		{
			SPSurfaceTypeDefault variationDefaults = threadState->getSurfaceDefaultsForVariationType(threadState, terrainVariation_deepWater);
			result.pathDifficultyIndex = variationDefaults.pathDifficultyIndex;
			variations[result.variationCount++] = terrainVariation_deepWater;
		}
		else
		{
			SPSurfaceTypeDefault variationDefaults = threadState->getSurfaceDefaultsForVariationType(threadState, terrainVariation_shallowWater);
			result.pathDifficultyIndex = variationDefaults.pathDifficultyIndex;
			variations[result.variationCount++] = terrainVariation_shallowWater;
		}
	}

	if(baseAltitude < SEA_LEVEL)
	{
		result.surfaceBaseType = (fillSurfaceBaseType != 0 ? fillSurfaceBaseType : getBeachSurfaceType(&surfaceTypeInfo, riverDistance, noiseValue, digFillOffset));

		SPSurfaceTypeDefault defaults = threadState->getSurfaceDefaultsForBaseType(threadState, result.surfaceBaseType);

		result.materialIndex = defaults.materialIndex;
		result.decalTypeIndex = 0;
		result.pathDifficultyIndex = defaults.pathDifficultyIndex;

		return result;
	}*/

	bool snowRemoved = false;
	bool shouldAddVegetation = true;
	int soilQuality = 1;

	for(int i = 0; i < modificationCount; i++)
	{
		if(modifications[i] == terrainModifcation_snowRemoved)
		{
			snowRemoved = true;
		}
		else if(modifications[i] == terrainModifcation_vegetationRemoved)
		{
			shouldAddVegetation = false;
		}
	}

	bool underWater = (baseAltitude + SP_METERS_TO_PRERENDER((double)digFillOffset) < SEA_LEVEL);
	if(underWater)
	{
		shouldAddVegetation = false;
		snowRemoved = true;
	}


	if(fillSurfaceBaseType != 0)
	{
		result.surfaceBaseType = fillSurfaceBaseType;

		if(shouldAddVegetation)
		{
			if(fillSurfaceBaseType == terrainBaseType_dirt)
			{
				soilQuality = 1;
			}
			else if(fillSurfaceBaseType == terrainBaseType_richDirt)
			{
				soilQuality = 2;
			}
			else if(fillSurfaceBaseType == terrainBaseType_poorDirt)
			{
				soilQuality = 1;
			}
			else
			{
				shouldAddVegetation = false;
			}
		}
	}
	else if(baseAltitude < SEA_LEVEL)
	{
		result.surfaceBaseType = getBeachSurfaceType(&surfaceTypeInfo, riverDistance, noiseValue, digFillOffset);
	}
	else
	{
		bool hasClay = (noiseValueMed > 0.1 && noiseValue < 0.2 + noiseValueSmall * 0.5);

		bool isBeach = ((baseAltitude + noiseValue * 0.00000005 + noiseValueLarge * 0.0000005) < 0.0000001);
		bool isRock = (steepness > rockSteepness + noiseValue * 0.5);
		bool isClay = hasClay && !isRock && (steepness > claySteepness + noiseValue * 0.5 - (1.0 - riverDistance) * (1.0 - riverDistance) * 0.5);
		bool isDesertSand = (soilRichnessNoiseValue < -0.65);
		bool hasSand = (!hasClay && soilRichnessNoiseValue < -0.45);

		bool isLimestone = getIsLimestone();
		bool isRedRock = getIsRedRock();
		bool isGreenRock = getIsGreenRock();

		if(digFillOffset != 0 && !isRock)
		{
			if(digFillOffset < (noiseValue * 4) - 2)
			{
				isRock = true;
			}
			else if(hasClay && !isClay)
			{
				if(digFillOffset < (noiseValueMed * 4) - 1)
				{
					isClay = true;
				}
			}
			else if(hasSand && !isDesertSand)
			{
				if(digFillOffset < (noiseValueMed * 4) - 1)
				{
					isDesertSand = true;
				}
			}
		}

		bool isRainforest = false;

		for(int i = 0; i < tagCount; i++)
		{
			if(tags[i] == biomeTag_cliff)
			{
				isRock = true;
			}
			else if(tags[i] == biomeTag_rainforest)
			{
				isRainforest = true;
			}
		}

		if(isRock)
		{
			result.surfaceBaseType = terrainBaseType_rock;
			if(isLimestone)
			{
				result.surfaceBaseType = terrainBaseType_limestone;
			}
			else if(isRedRock)
			{
				result.surfaceBaseType = terrainBaseType_redRock;
			}
			else if(isGreenRock)
			{
				result.surfaceBaseType = terrainBaseType_greenRock;
			}
		}
		else if(isClay)
		{
			result.surfaceBaseType = terrainBaseType_clay;
		}
		else if(isBeach)
		{
			result.surfaceBaseType = getBeachSurfaceType(&surfaceTypeInfo, riverDistance, noiseValue, digFillOffset);
		}
		else if(isDesertSand)
		{
			result.surfaceBaseType = terrainBaseType_desertSand;
			soilQuality = 0;
		}
		else
		{
			if(soilRichnessNoiseValue < -0.3)
			{
				if(isRainforest)
				{
					result.surfaceBaseType = terrainBaseType_dirt;
					soilQuality = 1;
				}
				else
				{
					result.surfaceBaseType = terrainBaseType_poorDirt;
					soilQuality = 0;
				}
			}
			else if(isRainforest || soilRichnessNoiseValue > 0.4)
			{
				result.surfaceBaseType = terrainBaseType_richDirt;
				soilQuality = 2;
			}
			else
			{
				result.surfaceBaseType = terrainBaseType_dirt;
				soilQuality = 1;
			}
		}


		for(int i = 0; i < tagCount; i++)
		{
			if(tags[i] == biomeTag_desert)
			{
				if(!isClay && !isRock)
				{
					if(surfaceTypeInfo.hot)
					{
						result.surfaceBaseType =  terrainBaseType_desertRedSand;
					}
					else
					{
						result.surfaceBaseType = terrainBaseType_desertSand;
					}
				}
			}
			else if(tags[i] == biomeTag_icecap)
			{
				if(!isRock && !isClay)
				{
					result.surfaceBaseType = terrainBaseType_ice;
				}
			}
		}

		shouldAddVegetation = shouldAddVegetation && (!isRock && !isBeach && !isClay && !isDesertSand);


		if(isLimestone)
		{
			variations[result.variationCount++] = terrainVariation_limestone;
			if(noiseValueMed > 0.1 && soilRichnessNoiseValue < 0.2 && noiseValue < 0.1)
			{
				variations[result.variationCount++] = terrainVariation_flint;
			}
		}
		if(isRedRock)
		{
			variations[result.variationCount++] = terrainVariation_redRock;
		}
		if(isGreenRock)
		{
			variations[result.variationCount++] = terrainVariation_greenRock;
		}

		if(hasClay)
		{
			variations[result.variationCount++] = terrainVariation_clay;
		}
	}

	uint32_t grassVariation = 0;

	if (shouldAddVegetation)
	{
		for (int i = 0; i < tagCount; i++)
		{
			if (tags[i] == biomeTag_steppe)
			{
				if(soilQuality == 0)
				{
					grassVariation = 0;
				}
				else if(soilQuality == 1)
				{
					grassVariation = terrainVariation_mediterraneanGrass;
				}
				else
				{
					grassVariation = terrainVariation_mediterraneanGrassPlentiful;
				}
			}
			else if (tags[i] == biomeTag_rainforest)
			{
				if(soilQuality == 2)
				{
					grassVariation = terrainVariation_tropicalRainforestGrassPlentiful;
				}
				else
				{
					grassVariation = terrainVariation_tropicalRainforestGrass;
				}
			}
			if (tags[i] == biomeTag_savanna)
			{
				grassVariation = terrainVariation_savannaGrass;
			}
			if (tags[i] == biomeTag_tundra)
			{
				grassVariation = terrainVariation_tundraGrass;
			}
			if (tags[i] == biomeTag_temperate)
			{
				if (seasonIndex == 0 || seasonIndex == 1)
				{
					if(soilQuality == 0)
					{
						grassVariation = terrainVariation_tundraGrass;
					}
					else if(soilQuality == 1)
					{
						grassVariation = terrainVariation_temperateGrass;
					}
					else
					{
						grassVariation = terrainVariation_temperateGrassPlentiful;
					}
				}
				else if (seasonIndex == 2)
				{
					if(soilQuality == 0)
					{
						grassVariation = 0;
					}
					else if(soilQuality == 1)
					{
						grassVariation = terrainVariation_mediterraneanGrass;
					}
					else
					{
						grassVariation = terrainVariation_mediterraneanGrassPlentiful;
					}
				}
				else
				{
					if(soilQuality == 0)
					{
						grassVariation = terrainVariation_tundraGrass;
					}
					else if(soilQuality == 1)
					{
						grassVariation = terrainVariation_temperateGrassWinter;
					}
					else
					{
						grassVariation = terrainVariation_temperateGrass;
					}
				}
			}
		}
	}

	bool hasSnow = false;
	if(!snowRemoved && surfaceTypeInfo.snowDepth > 0)
	{
		if(surfaceTypeInfo.snowDepth == 3)
		{
			hasSnow = true;
		}
		else if((noiseValue > -0.1))
		{
			if(surfaceTypeInfo.snowDepth == 2 || (noiseValue > 0.3))
			{
				hasSnow = true;
			}
		}
	}



	if(grassVariation != 0)
	{
		variations[result.variationCount++] = grassVariation;
		if(hasSnow)
		{
			variations[result.variationCount++] = terrainVariation_grassSnow;
		}
	}
	
	if(hasSnow)
	{
		variations[result.variationCount++] = terrainVariation_snow;
	}

	result.materialIndex = 0;

	if(grassVariation != 0 || hasSnow)
	{
		SPSurfaceTypeDefault variationDefaults = threadState->getSurfaceDefaultsForVariationType(threadState, (hasSnow ? (grassVariation != 0 ? terrainVariation_grassSnow : terrainVariation_snow) : grassVariation));
		if(variationDefaults.materialIndex != 0)
		{
			result.materialIndex = variationDefaults.materialIndex;
			result.pathDifficultyIndex = variationDefaults.pathDifficultyIndex;
			if(hasSnow)
			{
				if(grassVariation != 0)
				{
					result.decalTypeIndex = variationDefaults.decalGroupIndex;
				}
				else
				{
					result.decalTypeIndex = 0;
				}
			}
			else
			{
				result.decalTypeIndex = variationDefaults.decalGroupIndex;
			}
		}
	}
	
	if(result.materialIndex == 0)
	{
		SPSurfaceTypeDefault defaults = threadState->getSurfaceDefaultsForBaseType(threadState, result.surfaceBaseType);
		result.materialIndex = defaults.materialIndex;
		result.decalTypeIndex = defaults.decalGroupIndex;
		result.pathDifficultyIndex = defaults.pathDifficultyIndex;
	}

	if(baseAltitude + SP_METERS_TO_PRERENDER((double)digFillOffset) < SEA_LEVEL)
	{
		if(baseAltitude < DEEP_SEA_LEVEL)
		{
			SPSurfaceTypeDefault variationDefaults = threadState->getSurfaceDefaultsForVariationType(threadState, terrainVariation_deepWater);
			result.pathDifficultyIndex = variationDefaults.pathDifficultyIndex;
			variations[result.variationCount++] = terrainVariation_deepWater;
		}
		else
		{
			SPSurfaceTypeDefault variationDefaults = threadState->getSurfaceDefaultsForVariationType(threadState, terrainVariation_shallowWater);
			result.pathDifficultyIndex = variationDefaults.pathDifficultyIndex;
			variations[result.variationCount++] = terrainVariation_shallowWater;
		}
	}

	return result;
}

bool getHasSingleTag(uint16_t* biomeTags, int tagCount, uint16_t tag)
{
	for(int i = 0; i < tagCount; i++)
	{
		if(biomeTags[i] == tag)
		{
			return true;
		}
	}
	return false;
}

typedef struct ForestInfo {
	int forestDensity;
	bool coniferous;
	bool birch;
	bool cold;
	bool river;
	bool tropical;
	bool bamboo;

} ForestInfo;


void getForestInfo(uint16_t* biomeTags, 
	int tagCount, 
	double steepness,
	ForestInfo* forestInfo)
{
	for(int i = 0; i < tagCount; i++)
	{
		if(biomeTags[i] == biomeTag_cliff || steepness > 1.0)
		{
			forestInfo->forestDensity = 0;
			forestInfo->coniferous = false;
			forestInfo->birch = false;
			forestInfo->bamboo = false;
			return;
		}
		else if(biomeTags[i] == biomeTag_river)
		{
			forestInfo->river = true;
		}
		else if(biomeTags[i] == biomeTag_denseForest)
		{
			forestInfo->forestDensity = 4;
		}
		else if(biomeTags[i] == biomeTag_mediumForest)
		{
			forestInfo->forestDensity = 3;
		}
		else if(biomeTags[i] == biomeTag_sparseForest)
		{
			forestInfo->forestDensity = 2;
		}
		else if(biomeTags[i] == biomeTag_verySparseForest)
		{
			forestInfo->forestDensity = 1;
		}
		else if(biomeTags[i] == biomeTag_coniferous)
		{
			forestInfo->coniferous = true;
		}
		else if(biomeTags[i] == biomeTag_birch)
		{
			forestInfo->birch = true;
		}
		else if(biomeTags[i] == biomeTag_bamboo)
		{
			forestInfo->bamboo = true;
		}
		else if(biomeTags[i] == biomeTag_tropical)
		{
			forestInfo->tropical = true;
		}
		else if(biomeTags[i] == biomeTag_polar ||
			biomeTags[i] == biomeTag_temperatureWinterVeryCold)
		{
			forestInfo->cold = true;
		}
	}
}

#define ADD_OBJECT(__addType__)\
types[addedCount++] = __addType__;\
if(addedCount >= BIOME_MAX_GAME_OBJECT_COUNT_PER_SUBDIVISION)\
{\
	return addedCount;\
}

uint32_t getPineType(uint64_t faceUniqueID, int i)
{
	int randomValue = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 22245 + i, 12);

	/*if(randomValue == 3)
	{
		return gameObjectType_pineBig1;
	}*/
	if(randomValue == 1)
	{
		return gameObjectType_tallPine;
	}
	if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 22245 + i, 12) == 1)
	{
		return gameObjectType_tallPine;
	}
	return gameObjectType_medPineTypes[spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3434 + i, MED_PINE_TYPE_COUNT)];
}

uint32_t getBirchType(uint64_t faceUniqueID, int i)
{
	return gameObjectType_birchTypes[spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3434 + i, BIRCH_TYPE_COUNT)];
}

uint32_t getAspenType(uint64_t faceUniqueID, int i)
{
	return gameObjectType_aspenTypes[spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3434 + i, ASPEN_TYPE_COUNT)];
}

uint32_t getWillowType(uint64_t faceUniqueID, int i)
{
	return gameObjectType_willowTypes[spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3434 + i, WILLOW_TYPE_COUNT)];
}

uint32_t getBambooType(uint64_t faceUniqueID, int i)
{
	return gameObjectType_bambooTypes[spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3434 + i, BAMBOO_TYPE_COUNT)];
}

uint32_t getTreeType(uint64_t faceUniqueID, int i, ForestInfo* forestInfo, double noiseValueMed)
{

	if(forestInfo->river)
	{
		if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 342573 + i, 2) == 1)
		{
			return getWillowType(faceUniqueID, i);
		}
	}

	if(forestInfo->coniferous)
	{
		if(forestInfo->birch)
		{
			int randomValue = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 54634 + i, 2);
			if(randomValue == 1)
			{
				return getPineType(faceUniqueID, i);
			}
			else if(noiseValueMed > 0.2)
			{
				return getAspenType(faceUniqueID, i);
			}
			return getBirchType(faceUniqueID, i);
		}
		else
		{
			return getPineType(faceUniqueID, i);
		}
	}

	if(forestInfo->birch)
	{
		if(noiseValueMed > 0.2)
		{
			return getAspenType(faceUniqueID, i);
		}
		return getBirchType(faceUniqueID, i);
	}

	return getPineType(faceUniqueID, i);
}


int spBiomeGetTransientGameObjectTypesForFaceSubdivision(SPBiomeThreadState* threadState,
	int incomingTypeCount,
	uint32_t* types,
	uint16_t* biomeTags,
	int tagCount,
	SPVec3 pointNormal, 
	SPVec3 noiseLoc, 
	uint64_t faceUniqueID, 
	int level, 
	double altitude, 
	double steepness,
	double riverDistance)
{
	int addedCount = incomingTypeCount;

	if(addedCount < BIOME_MAX_GAME_OBJECT_COUNT_PER_SUBDIVISION)
	{
		if(altitude > -0.0000001)
		{
			if(level >= SP_SUBDIVISIONS - 7)
			{
				SPVec3 noiseLookup = spVec3Mul(noiseLoc, 999.0);

				if(level == SP_SUBDIVISIONS - 7)
				{
					ForestInfo forestInfo;
					memset(&forestInfo, 0, sizeof(forestInfo));
					getForestInfo(biomeTags,
						tagCount,
						steepness,
						&forestInfo);

					if(forestInfo.forestDensity >= 3)
					{
						if(forestInfo.coniferous)
						{
							if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 60638, 16) == 3)
							{
								ADD_OBJECT(gameObjectType_pineBig1);
							}
						}

						if(forestInfo.birch)
						{
							if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 95935, 8) == 4)
							{

								SPVec3 scaledNoiseMedScale = spVec3Mul(noiseLoc, 92273.0);
								double noiseValueMed = spNoiseGet(threadState->spNoise1, scaledNoiseMedScale, 2);
								if(noiseValueMed > 0.2)
								{
									ADD_OBJECT(gameObjectType_aspenBig1);
								}
							}
						}
					}
				}
				else if(level == SP_SUBDIVISIONS - 6)
				{
					ForestInfo forestInfo;
					memset(&forestInfo, 0, sizeof(forestInfo));
					getForestInfo(biomeTags,
						tagCount,
						steepness,
						&forestInfo);


					int treeCount = 0;
					if(forestInfo.coniferous || forestInfo.birch)
					{
						switch(forestInfo.forestDensity)
						{
						case 0:
							break;
						case 1:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 16) - 14;
							break;
						case 2:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) - 2;
							break;
						case 3:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) + 4;
							break;
						case 4:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) + 24;
							break;

						}

						if(treeCount > 0)
						{
							/*if(forestInfo.birch)
							{
								if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 100 / treeCount) == 1)
								{
									ADD_OBJECT(gameObjectType_aspen3);
								}
							}*/


							SPVec3 scaledNoiseMedScale = spVec3Mul(noiseLoc, 92273.0);
							double noiseValueMed = spNoiseGet(threadState->spNoise1, scaledNoiseMedScale, 2);

							for(int i = 0; i < treeCount; i++)
							{
								ADD_OBJECT(getTreeType(faceUniqueID, i, &forestInfo, noiseValueMed));
							}
						}
					}
				}
				else if(level == SP_SUBDIVISIONS - 4)
				{
					ForestInfo forestInfo;
					memset(&forestInfo, 0, sizeof(forestInfo));
					getForestInfo(biomeTags,
						tagCount,
						steepness,
						&forestInfo);


					if(forestInfo.forestDensity > 0)
					{
						SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 610.0);
						double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);

						if(noiseValue > 0.5)
						{
							uint32_t treeObjectType = gameObjectType_appleTree;

							if(forestInfo.tropical)
							{
								treeObjectType = gameObjectType_bananaTree;
							}
							int objectCount = (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 9235, 18) - 6);
							for(int i = 0; i < objectCount; i++)
							{

								ADD_OBJECT(treeObjectType);
							}
						}
						else if(noiseValue < -0.5)
						{
							if(!forestInfo.cold)
							{
								int objectCount = (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 9822, 18) - 6);
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_orangeTree);
								}
							}
							else
							{
								int objectCount = (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 232, 18) - 6);
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_peachTree);
								}
							}
						}

						SPVec3 offsetB = {0.32,0.67,0.31};
						scaledNoiseLoc = spVec3Mul(spVec3Add(noiseLookup, offsetB), 245.3);
						noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
						if(noiseValue > 0.6)
						{
							uint32_t treeObjectType = gameObjectType_elderberryTree;

							int objectCount = (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 9235, 18) - 6);
							for(int i = 0; i < objectCount; i++)
							{

								ADD_OBJECT(treeObjectType);
							}
						}

						if(forestInfo.tropical)
						{
							SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 762.0);
							double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);

							if(noiseValue > 0.5)
							{
								int objectCount = (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 15234, 18) - 6);
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_coconutTree);
								}
							}
						}

						if(forestInfo.coniferous || forestInfo.birch)
						{
							int treeCount = 0;
							switch(forestInfo.forestDensity)
							{
							case 1:
								treeCount = (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 32) == 1 ? 1 : 0);
								break;
							case 2:
								treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 16) - 14;
								break;
							case 3:
								treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) - 6;
								break;
							case 4:
								treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) - 2;
								break;
							default:
								break;
							}

							for(int i = 0; i < treeCount; i++)
							{
								bool addPine = (!forestInfo.birch || (forestInfo.coniferous && (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 9238 + i, 2) == 0)));

								if(addPine)
								{
									ADD_OBJECT(gameObjectType_smallPine);
								}
								else
								{
									ADD_OBJECT(getBirchType(faceUniqueID, i));
								}
							}
						}


						if(forestInfo.bamboo && spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 236604, 6) == 0)
						{
							int treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 91333, 7) + forestInfo.forestDensity + 2;

							for(int i = 0; i < treeCount; i++)
							{
								ADD_OBJECT(getBambooType(faceUniqueID, i));
							}
						}
					}


					SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 400.0);
					double rawValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
					double rangedFractionValue = rawValue * rawValue * 8.0;
					if(riverDistance < (rawValue * 0.01 + 0.005))
					{
						rangedFractionValue += 1.0;
					}
					else if(forestInfo.river)
					{
						rangedFractionValue += 0.5;
					}

					int addBoulderCount = (((int)spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 83637, 60)) - 58 + 5 * rangedFractionValue);
					if(addBoulderCount > 0)
					{
						uint32_t rockType = gameObjectType_rockLarge;


						SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 45999.0);
						double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
						SPVec3 scaledNoiseMedScale = spVec3Mul(noiseLoc, 92273.0);
						double noiseValueMed = spNoiseGet(threadState->spNoise1, scaledNoiseMedScale, 2);
						SPVec3 scaledNoiseLocSmallScale = spVec3Mul(noiseLoc, 834567.0);
						double noiseValueSmall = spNoiseGet(threadState->spNoise1, scaledNoiseLocSmallScale, 2);

						bool isLimestone = getIsLimestone();
						bool isRedRock = getIsRedRock();
						bool isGreenRock = getIsGreenRock();

						if(isLimestone)
						{
							rockType = gameObjectType_limestoneRockLarge;
						}
						else if(isRedRock)
						{
							rockType = gameObjectType_redRockLarge;
						}
						else if(isGreenRock)
						{
							rockType = gameObjectType_greenRockLarge;
						}

						for(int i = 0; i < addBoulderCount; i++)
						{
							ADD_OBJECT(rockType);
						}
					}

				}
				else if(level == SP_SUBDIVISIONS - 3)
				{
					/*bool temperate = false;
					bool steppe = false;

					for(int i = 0; i < tagCount; i++)
					{
						if(biomeTags[i] == biomeTag_temperate)
						{
							temperate = true;
						}
						else if(biomeTags[i] == biomeTag_steppe)
						{
							steppe = true;
						}
					}*/

					//if(temperate || steppe)
					{

						ForestInfo forestInfo;
						memset(&forestInfo, 0, sizeof(forestInfo));
						getForestInfo(biomeTags,
							tagCount,
							steepness,
							&forestInfo);

						int objectCount = 0;


						if(forestInfo.bamboo && forestInfo.forestDensity > 0 && spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 236604, 12) == 0)
						{
							int treeCount = (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 8137, 14) + forestInfo.forestDensity) / 2;

							for(int i = 0; i < treeCount; i++)
							{
								ADD_OBJECT(gameObjectType_smallBamboo);
							}
						}

						if(forestInfo.forestDensity > 0)
						{
							if(forestInfo.forestDensity > 2)
							{
								objectCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 235432, 12) - 4;
							}
							else
							{
								objectCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 235432, 36) - 28;
							}
						}

						if(objectCount > 0)
						{
							double soilRichnessNoiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance);
							if(soilRichnessNoiseValue < 0.0)
							{
								objectCount = objectCount + soilRichnessNoiseValue * 10.0;
							}
							

							if(objectCount > 0)
							{
								SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 129.0);
								double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);

								if(noiseValue > 0.5)
								{
									//if(temperate)
									{
										for(int i = 0; i < objectCount; i++)
										{
											ADD_OBJECT(gameObjectType_sunflower);
										}
									}
								}
								else if(noiseValue < -0.5)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_raspberryBush);
									}
								}

								SPVec3 offset = {0.2,0.1,0.3};
								scaledNoiseLoc = spVec3Mul(spVec3Add(noiseLookup, offset), 124.2);
								noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2); 
								if(noiseValue > 0.5)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_gooseberryBush);
									}
								}
								else if(noiseValue < -0.5)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_beetrootPlant);
									}
								}

								SPVec3 offsetB = {0.6,0.22,0.5};
								scaledNoiseLoc = spVec3Mul(spVec3Add(noiseLookup, offsetB), 134.7);
								noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2); 
								if(noiseValue > 0.5)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_wheatPlant);
									}
								}
								else if(noiseValue < -0.3)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_flaxPlant);
									}
								}

								SPVec3 offsetC = {0.41,0.17,0.13};
								scaledNoiseLoc = spVec3Mul(spVec3Add(noiseLookup, offsetC), 121.2);
								noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2); 
								if(noiseValue > 0.5)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_pumpkinPlant);
									}
								}


								SPVec3 offsetD = {0.53,0.48,0.72};
								scaledNoiseLoc = spVec3Mul(spVec3Add(noiseLookup, offsetD), 72.3);
								noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2); 
								if(noiseValue > 0.55)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_gingerPlant);
									}
								}
								else if(noiseValue < -0.55)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_marigoldPlant);
									}
								}
								else if(noiseValue > -0.005 && noiseValue < 0.005)
								{
									int randomTypeIndex = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID & 0xFFFFFFFFFFC00000, 1756, 3);
									uint32_t typeToAdd = gameObjectType_turmericPlant;
									if(randomTypeIndex == 1)
									{
										typeToAdd = gameObjectType_echinaceaPlant;
									}
									else if(randomTypeIndex == 2)
									{
										typeToAdd = gameObjectType_garlicPlant;
									}
									
									for(int i = 0; i < spMax(objectCount / 2, 1); i++)
									{
										ADD_OBJECT(typeToAdd);
									}
								}


								SPVec3 offsetE = {0.26,0.59,0.88};
								scaledNoiseLoc = spVec3Mul(spVec3Add(noiseLookup, offsetD), 79.3);
								noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2); 
								if(noiseValue > 0.55)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_aloePlant);
									}
								}
								else if(noiseValue < -0.55)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_poppyPlant);
									}
								}
							}
						}
					}

					bool addCarcass = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 73958, 400) == 1;
					if(addCarcass == 1)
					{
						uint32_t carcassType = gameObjectType_deadAlpaca;
						uint32_t randomCarcassTypeValue = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3966, 20);
						if(randomCarcassTypeValue < 6)
						{
							carcassType = gameObjectType_bone;
						}
						else if(randomCarcassTypeValue == 6)
						{
							carcassType = gameObjectType_deadMammoth;
						}
						ADD_OBJECT(carcassType);
					}
				}
				else if(level == SP_SUBDIVISIONS - 2)
				{
					ForestInfo forestInfo;
					memset(&forestInfo, 0, sizeof(forestInfo));
					getForestInfo(biomeTags,
						tagCount,
						steepness,
						&forestInfo);

					SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 400.0);
					double rawValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
					double rangedFractionValue = rawValue * rawValue * 8.0;
					if(riverDistance < (rawValue * 0.01 + 0.005))
					{
						rangedFractionValue += 1.0;
					}
					else if(forestInfo.river)
					{
						rangedFractionValue += 0.5;
					}
					int objectCount = ((int)spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 1324, 20)) - 20 + 4 * rangedFractionValue;
					if(objectCount > 0)
					{
						uint32_t rockType = gameObjectType_rock;


						SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 45999.0);
						double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
						SPVec3 scaledNoiseMedScale = spVec3Mul(noiseLoc, 92273.0);
						double noiseValueMed = spNoiseGet(threadState->spNoise1, scaledNoiseMedScale, 2);
						SPVec3 scaledNoiseLocSmallScale = spVec3Mul(noiseLoc, 834567.0);
						double noiseValueSmall = spNoiseGet(threadState->spNoise1, scaledNoiseLocSmallScale, 2);

						bool isLimestone = getIsLimestone();
						bool isRedRock = getIsRedRock();
						bool isGreenRock = getIsGreenRock();


						bool hasClay = (noiseValueMed > 0.1 && noiseValue < 0.2 + noiseValueSmall * 0.5);


						if(hasClay)
						{
							int halfCount = objectCount / 4;
							halfCount = halfCount < 1 ? 1 : halfCount;

							for(int i = 0; i < halfCount; i++)
							{
								ADD_OBJECT(gameObjectType_clay);
							}
						}

						if(isLimestone)
						{
							rockType = gameObjectType_limestoneRock;
						}
						else if(isRedRock)
						{
							rockType = terrainVariation_redRock;
						}
						else if(isGreenRock)
						{
							rockType = terrainVariation_greenRock;
						}

						for(int i = 0; i < objectCount; i++)
						{
							ADD_OBJECT(rockType);
						}
					}

					if(forestInfo.forestDensity > 0)
					{
						int denominatorMultiplier = 0;
						if(forestInfo.coniferous)
						{
							denominatorMultiplier++;
						}
						if(forestInfo.birch)
						{
							denominatorMultiplier++;
						}
						if(forestInfo.bamboo)
						{
							denominatorMultiplier++;
						}
						denominatorMultiplier = spMax(denominatorMultiplier, 1);

						int denominator = (200 / (forestInfo.forestDensity * forestInfo.forestDensity)) * denominatorMultiplier;
						denominator = spMax(denominator, 4);
						int roll = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 235432, denominator);
						if(roll == 0)
						{
							if(forestInfo.coniferous)
							{
								ADD_OBJECT(gameObjectType_pineBranch);
							}

							if(forestInfo.birch)
							{
								ADD_OBJECT(gameObjectType_birchBranch);
							}

							if(forestInfo.bamboo)
							{
								ADD_OBJECT(gameObjectType_bambooBranch);
							}
							if(forestInfo.river)
							{
								ADD_OBJECT(gameObjectType_willowBranch);
							}
						}
					}

				}
				else if(level == SP_SUBDIVISIONS - 1)
				{

					SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 4000.0);
					double rawValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
					double rangedFractionValue = rawValue * rawValue * 8.0;

					if(riverDistance < (rawValue * 0.01 + 0.005))
					{
						rangedFractionValue += 1.0;
					}
					else if(getHasSingleTag(biomeTags, tagCount, biomeTag_river))
					{
						rangedFractionValue += 0.5;
					}
					int objectCount = (((int)spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 5243, 40)) - 38 + 2 * rangedFractionValue) / 2;

					if(objectCount > 0)
					{
						uint32_t rockType = gameObjectType_rockSmall;

						SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 45999.0);
						double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
						SPVec3 scaledNoiseMedScale = spVec3Mul(noiseLoc, 92273.0);
						double noiseValueMed = spNoiseGet(threadState->spNoise1, scaledNoiseMedScale, 2);
						SPVec3 scaledNoiseLocSmallScale = spVec3Mul(noiseLoc, 834567.0);
						double noiseValueSmall = spNoiseGet(threadState->spNoise1, scaledNoiseLocSmallScale, 2);

						bool isLimestone = getIsLimestone();
						bool isRedRock = getIsRedRock();
						bool isGreenRock = getIsGreenRock();

						if(isLimestone)
						{
							double soilRichnessNoiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance);
							if(noiseValueMed > 0.1 && soilRichnessNoiseValue < 0.2 && noiseValue < 0.1)
							{

								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_flint);
								}
							}

							rockType = gameObjectType_limestoneRockSmall;
						}
						else if(isRedRock)
						{
							rockType = gameObjectType_redRockSmall;
						}
						else if(isGreenRock)
						{
							rockType = gameObjectType_greenRockSmall;
						}

						for(int i = 0; i < objectCount; i++)
						{
							ADD_OBJECT(rockType);
						}
					}
				}
			}
		}
	}
	return addedCount;
}
