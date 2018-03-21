#!/bin/python

# Requires asetup/panda 
# Latest used : asetup 20.1.8.3,AtlasDerivation,gcc48,here (09/05/2016) 

datasets=(
'mc15_13TeV.389940.MGPy8EG_A14N23LO_TT_bWN_200_110_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389941.MGPy8EG_A14N23LO_TT_bWN_225_135_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389942.MGPy8EG_A14N23LO_TT_bWN_225_075_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389943.MGPy8EG_A14N23LO_TT_bWN_250_100_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389944.MGPy8EG_A14N23LO_TT_bWN_250_130_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389945.MGPy8EG_A14N23LO_TT_bWN_250_160_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389946.MGPy8EG_A14N23LO_TT_bWN_275_125_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389947.MGPy8EG_A14N23LO_TT_bWN_275_155_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389948.MGPy8EG_A14N23LO_TT_bWN_275_185_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389949.MGPy8EG_A14N23LO_TT_bWN_300_150_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389950.MGPy8EG_A14N23LO_TT_bWN_300_180_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389951.MGPy8EG_A14N23LO_TT_bWN_300_210_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389952.MGPy8EG_A14N23LO_TT_bWN_325_175_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389953.MGPy8EG_A14N23LO_TT_bWN_325_205_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389954.MGPy8EG_A14N23LO_TT_bWN_325_235_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389955.MGPy8EG_A14N23LO_TT_bWN_350_200_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389956.MGPy8EG_A14N23LO_TT_bWN_350_230_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389957.MGPy8EG_A14N23LO_TT_bWN_350_260_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389958.MGPy8EG_A14N23LO_TT_bWN_375_225_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389959.MGPy8EG_A14N23LO_TT_bWN_375_255_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389960.MGPy8EG_A14N23LO_TT_bWN_375_285_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389961.MGPy8EG_A14N23LO_TT_bWN_400_250_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389962.MGPy8EG_A14N23LO_TT_bWN_400_280_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389963.MGPy8EG_A14N23LO_TT_bWN_400_310_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389964.MGPy8EG_A14N23LO_TT_bWN_425_275_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389965.MGPy8EG_A14N23LO_TT_bWN_425_305_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389966.MGPy8EG_A14N23LO_TT_bWN_425_335_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389967.MGPy8EG_A14N23LO_TT_bWN_450_300_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389968.MGPy8EG_A14N23LO_TT_bWN_450_330_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389969.MGPy8EG_A14N23LO_TT_bWN_450_360_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389970.MGPy8EG_A14N23LO_TT_bWN_475_325_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389971.MGPy8EG_A14N23LO_TT_bWN_475_355_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389972.MGPy8EG_A14N23LO_TT_bWN_475_385_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389973.MGPy8EG_A14N23LO_TT_bWN_500_350_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389974.MGPy8EG_A14N23LO_TT_bWN_500_380_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389975.MGPy8EG_A14N23LO_TT_bWN_500_410_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389976.MGPy8EG_A14N23LO_TT_bWN_525_375_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389977.MGPy8EG_A14N23LO_TT_bWN_525_405_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389978.MGPy8EG_A14N23LO_TT_bWN_525_435_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389979.MGPy8EG_A14N23LO_TT_bWN_550_400_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389980.MGPy8EG_A14N23LO_TT_bWN_550_430_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389981.MGPy8EG_A14N23LO_TT_bWN_550_460_MadSpin_2L15.evgen.EVNT.e5563',
#'mc15_13TeV.389982.MGPy8EG_A14N23LO_TT_bffN_250_200_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389983.MGPy8EG_A14N23LO_TT_bffN_250_220_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389984.MGPy8EG_A14N23LO_TT_bffN_275_195_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389985.MGPy8EG_A14N23LO_TT_bffN_275_225_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389986.MGPy8EG_A14N23LO_TT_bffN_275_245_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389987.MGPy8EG_A14N23LO_TT_bffN_300_220_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389988.MGPy8EG_A14N23LO_TT_bffN_300_250_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389989.MGPy8EG_A14N23LO_TT_bffN_300_270_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389990.MGPy8EG_A14N23LO_TT_bffN_325_245_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389991.MGPy8EG_A14N23LO_TT_bffN_325_275_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389992.MGPy8EG_A14N23LO_TT_bffN_325_295_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389993.MGPy8EG_A14N23LO_TT_bffN_350_270_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389994.MGPy8EG_A14N23LO_TT_bffN_350_300_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389995.MGPy8EG_A14N23LO_TT_bffN_350_320_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389996.MGPy8EG_A14N23LO_TT_bffN_375_295_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389997.MGPy8EG_A14N23LO_TT_bffN_375_325_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389998.MGPy8EG_A14N23LO_TT_bffN_375_345_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.389999.MGPy8EG_A14N23LO_TT_bffN_400_320_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390000.MGPy8EG_A14N23LO_TT_bffN_400_350_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390001.MGPy8EG_A14N23LO_TT_bffN_400_370_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390002.MGPy8EG_A14N23LO_TT_bffN_425_345_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390003.MGPy8EG_A14N23LO_TT_bffN_425_375_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390004.MGPy8EG_A14N23LO_TT_bffN_425_395_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390005.MGPy8EG_A14N23LO_TT_bffN_450_370_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390006.MGPy8EG_A14N23LO_TT_bffN_450_400_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390007.MGPy8EG_A14N23LO_TT_bffN_450_420_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390008.MGPy8EG_A14N23LO_TT_bffN_475_395_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390009.MGPy8EG_A14N23LO_TT_bffN_475_425_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390010.MGPy8EG_A14N23LO_TT_bffN_475_445_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390011.MGPy8EG_A14N23LO_TT_bffN_500_420_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390012.MGPy8EG_A14N23LO_TT_bffN_500_450_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390013.MGPy8EG_A14N23LO_TT_bffN_500_470_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390014.MGPy8EG_A14N23LO_TT_bffN_525_445_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390015.MGPy8EG_A14N23LO_TT_bffN_525_475_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390016.MGPy8EG_A14N23LO_TT_bffN_525_495_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390017.MGPy8EG_A14N23LO_TT_bffN_550_470_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390018.MGPy8EG_A14N23LO_TT_bffN_550_500_MadSpin_2L15.evgen.EVNT.e5562',
#'mc15_13TeV.390019.MGPy8EG_A14N23LO_TT_bffN_550_520_MadSpin_2L15.evgen.EVNT.e5562',
###'mc15_13TeV.387940.MGPy8EG_A14N23LO_TT_bWN_225_135_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387941.MGPy8EG_A14N23LO_TT_bWN_250_100_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387942.MGPy8EG_A14N23LO_TT_bWN_250_130_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387943.MGPy8EG_A14N23LO_TT_bWN_250_160_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387944.MGPy8EG_A14N23LO_TT_bWN_275_125_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387945.MGPy8EG_A14N23LO_TT_bWN_275_155_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387946.MGPy8EG_A14N23LO_TT_bWN_275_185_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387947.MGPy8EG_A14N23LO_TT_bWN_300_150_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387948.MGPy8EG_A14N23LO_TT_bWN_300_180_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387949.MGPy8EG_A14N23LO_TT_bWN_300_210_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387950.MGPy8EG_A14N23LO_TT_bWN_325_175_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387951.MGPy8EG_A14N23LO_TT_bWN_325_205_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387952.MGPy8EG_A14N23LO_TT_bWN_325_235_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387953.MGPy8EG_A14N23LO_TT_bWN_350_200_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387954.MGPy8EG_A14N23LO_TT_bWN_350_230_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387955.MGPy8EG_A14N23LO_TT_bWN_350_260_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387956.MGPy8EG_A14N23LO_TT_bWN_375_225_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387957.MGPy8EG_A14N23LO_TT_bWN_375_255_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387958.MGPy8EG_A14N23LO_TT_bWN_375_285_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387959.MGPy8EG_A14N23LO_TT_bWN_400_250_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387960.MGPy8EG_A14N23LO_TT_bWN_400_280_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387961.MGPy8EG_A14N23LO_TT_bWN_400_310_2L15.evgen.EVNT.e4898',
###'mc15_13TeV.387997.MGPy8EG_A14N23LO_TT_bWN_250_160_1L20orMET60.evgen.EVNT.e4908',
###'mc15_13TeV.387998.MGPy8EG_A14N23LO_TT_bWN_300_180_1L20orMET60.evgen.EVNT.e4908',
###'mc15_13TeV.387999.MGPy8EG_A14N23LO_TT_bWN_350_200_1L20orMET60.evgen.EVNT.e4908',
###'mc15_13TeV.406009.HppEG_UE5C6L1_Tt_bWN_t250_n160_2Lep18.evgen.EVNT.e4107',
###'mc15_13TeV.406010.HppEG_UE5C6L1_Tt_bWN_t300_n150_2Lep18.evgen.EVNT.e4107',
###'mc15_13TeV.406011.HppEG_UE5C6L1_Tt_bWN_t300_n180_2Lep18.evgen.EVNT.e4107',
)

import os

for dataset in datasets:
    print '===================================================================='
    print 'Submitting %s ...' % (dataset)
    print ''
    command=('pathena --trf "Reco_tf.py --inputEVNTFile=%%IN --outputDAODFile=output.pool.root --reductionConf TRUTH3" --inDS %s --outDS %s --extOutFile=DAOD_TRUTH3.output.pool.root' 
            % (dataset,dataset.replace('mc15_13TeV','user.amete').replace('evgen.EVNT','DAOD_TRUTH3')+str('.forHEPdata.v1')))
    os.system(command)
