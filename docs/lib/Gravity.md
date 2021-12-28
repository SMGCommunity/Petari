# Gravity
| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) 
| ------------- | ------------- | ------------- | ------------- |
| ConeGravity.o | 19.306930693069308% | 5 / 6 | 83.33333333333334% |
| CubeGravity.o | 1.3597033374536465% | 1 / 10 | 10.0% |
| DiskGravity.o | 33.33333333333333% | 9 / 11 | 81.81818181818183% |
| DiskTorusGravity.o | 20.99644128113879% | 7 / 9 | 77.77777777777779% |
| GlobalGravityObj.o | 100.0% | 18 / 18 | 100.0% |
| GraviryFollower.o | 100.0% | 4 / 4 | 100.0% |
| GravityCreator.o | 100.0% | 42 / 42 | 100.0% |
| GravityInfo.o | 100.0% | 2 / 2 | 100.0% |
| ParallelGravity.o | 40.430622009569376% | 9 / 13 | 69.23076923076923% |
| PlanetGravity.o | 100.0% | 9 / 9 | 100.0% |
| PlanetGravityManager.o | 21.73913043478261% | 3 / 5 | 60.0% |
| PointGravity.o | 100.0% | 3 / 3 | 100.0% |
| SegmentGravity.o | 25.115207373271893% | 5 / 8 | 62.5% |


# ConeGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11ConeGravityFv | :white_check_mark: |
| setLocalMatrix__11ConeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :white_check_mark: |
| setEnableBottom__11ConeGravityFb | :white_check_mark: |
| setTopCutRate__11ConeGravityFf | :white_check_mark: |
| updateMtx__11ConeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :white_check_mark: |
| calcOwnGravityVector__11ConeGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |


# CubeGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| isZero__Q29JGeometry8TVec3<f>CFv | :white_check_mark: |
| __ct__11CubeGravityFv | :x: |
| setCube__11CubeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| calcGravityArea__11CubeGravityCFRCQ29JGeometry8TVec3<f> | :x: |
| calcFaceGravity__11CubeGravityCFRCQ29JGeometry8TVec3<f>lPQ29JGeometry8TVec3<f>Pf | :x: |
| calcEdgeGravity__11CubeGravityCFRCQ29JGeometry8TVec3<f>lPQ29JGeometry8TVec3<f>Pf | :x: |
| calcCornerGravity__11CubeGravityCFRCQ29JGeometry8TVec3<f>lPQ29JGeometry8TVec3<f>Pf | :x: |
| calcOwnGravityVector__11CubeGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |
| updateMtx__11CubeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| normalize__Q29JGeometry8TVec3<f>FRCQ29JGeometry8TVec3<f> | :x: |


# DiskGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11DiskGravityFv | :white_check_mark: |
| setLocalPosition__11DiskGravityFRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| setLocalDirection__11DiskGravityFRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| setSideDirection__11DiskGravityFRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| setRadius__11DiskGravityFf | :white_check_mark: |
| setValidDegee__11DiskGravityFf | :white_check_mark: |
| setBothSide__11DiskGravityFb | :white_check_mark: |
| setEnableEdgeGravity__11DiskGravityFb | :white_check_mark: |
| calcOwnGravityVector__11DiskGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |
| updateMtx__11DiskGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :white_check_mark: |
| updateLocalParam__11DiskGravityFv | :x: |


# DiskTorusGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__16DiskTorusGravityFv | :white_check_mark: |
| setPosition__16DiskTorusGravityFRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| setDirection__16DiskTorusGravityFRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| setRadius__16DiskTorusGravityFf | :white_check_mark: |
| setDiskRadius__16DiskTorusGravityFf | :white_check_mark: |
| setEdgeType__16DiskTorusGravityFl | :white_check_mark: |
| setBothSide__16DiskTorusGravityFb | :white_check_mark: |
| updateMtx__16DiskTorusGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| calcOwnGravityVector__16DiskTorusGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |


# GlobalGravityObj.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__16GlobalGravityObjFPCc | :white_check_mark: |
| init__16GlobalGravityObjFRC12JMapInfoIter | :white_check_mark: |
| makeActorAppeared__16GlobalGravityObjFv | :white_check_mark: |
| makeActorDead__16GlobalGravityObjFv | :white_check_mark: |
| updateSwitch__16GlobalGravityObjFv | :white_check_mark: |
| movement__16GlobalGravityObjFv | :white_check_mark: |
| getGravity__16GlobalGravityObjFv | :white_check_mark: |
| createGlobalCubeGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalConeGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalDiskGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalDiskTorusGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalPlaneGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalPlaneInBoxGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalPlaneInCylinderGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalPointGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalSegmentGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalWireGravityObj__2MRFPCc | :white_check_mark: |
| __dt__16GlobalGravityObjFv | :white_check_mark: |


# GraviryFollower.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__15GraviryFollowerFP16GlobalGravityObjRC12JMapInfoIter | :white_check_mark: |
| setGravityFollowHost__15GraviryFollowerFPC7NameObj | :white_check_mark: |
| update__15GraviryFollowerFv | :white_check_mark: |
| addBaseMatrixFollowerGravity__2MRFP16GlobalGravityObjRC12JMapInfoIter | :white_check_mark: |


# GravityCreator.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__14GravityCreatorFv | :white_check_mark: |
| createFromJMap__14GravityCreatorFRC12JMapInfoIter | :white_check_mark: |
| createInstance__18CubeGravityCreatorFv | :white_check_mark: |
| settingFromSRT__18CubeGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__18CubeGravityCreatorFlll | :white_check_mark: |
| getGravity__18CubeGravityCreatorFv | :white_check_mark: |
| createInstance__18DiskGravityCreatorFv | :white_check_mark: |
| settingFromSRT__18DiskGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__18DiskGravityCreatorFlll | :white_check_mark: |
| getGravity__18DiskGravityCreatorFv | :white_check_mark: |
| createInstance__23DiskTorusGravityCreatorFv | :white_check_mark: |
| settingFromSRT__23DiskTorusGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__23DiskTorusGravityCreatorFlll | :white_check_mark: |
| getGravity__23DiskTorusGravityCreatorFv | :white_check_mark: |
| createInstance__18ConeGravityCreatorFv | :white_check_mark: |
| settingFromSRT__18ConeGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__18ConeGravityCreatorFlll | :white_check_mark: |
| getGravity__18ConeGravityCreatorFv | :white_check_mark: |
| createInstance__19PlaneGravityCreatorFv | :white_check_mark: |
| settingFromSRT__19PlaneGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| getGravity__19PlaneGravityCreatorFv | :white_check_mark: |
| createInstance__24PlaneInBoxGravityCreatorFv | :white_check_mark: |
| settingFromSRT__24PlaneInBoxGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| getGravity__24PlaneInBoxGravityCreatorFv | :white_check_mark: |
| settingFromJMapArgs__24PlaneInBoxGravityCreatorFlll | :white_check_mark: |
| createInstance__29PlaneInCylinderGravityCreatorFv | :white_check_mark: |
| getGravity__29PlaneInCylinderGravityCreatorFv | :white_check_mark: |
| settingFromSRT__29PlaneInCylinderGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__29PlaneInCylinderGravityCreatorFlll | :white_check_mark: |
| createInstance__19PointGravityCreatorFv | :white_check_mark: |
| settingFromSRT__19PointGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| getGravity__19PointGravityCreatorFv | :white_check_mark: |
| createInstance__21SegmentGravityCreatorFv | :white_check_mark: |
| settingFromSRT__21SegmentGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__21SegmentGravityCreatorFlll | :white_check_mark: |
| getGravity__21SegmentGravityCreatorFv | :white_check_mark: |
| createInstance__18WireGravityCreatorFv | :white_check_mark: |
| settingFromJMapOtherParam__18WireGravityCreatorFRC12JMapInfoIter | :white_check_mark: |
| getGravity__18WireGravityCreatorFv | :white_check_mark: |
| settingFromSRT__14GravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__14GravityCreatorFlll | :white_check_mark: |
| settingFromJMapOtherParam__14GravityCreatorFRC12JMapInfoIter | :white_check_mark: |


# GravityInfo.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11GravityInfoFv | :white_check_mark: |
| init__11GravityInfoFv | :white_check_mark: |


# ParallelGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__15ParallelGravityFv | :white_check_mark: |
| calcOwnGravityVector__15ParallelGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| updateMtx__15ParallelGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setPlane__15ParallelGravityFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| setRangeBox__15ParallelGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setRangeCylinder__15ParallelGravityFff | :white_check_mark: |
| setRangeType__15ParallelGravityFQ215ParallelGravity10RANGE_TYPE | :white_check_mark: |
| setBaseDistance__15ParallelGravityFf | :white_check_mark: |
| setDistanceCalcType__15ParallelGravityFQ215ParallelGravity18DISTANCE_CALC_TYPE | :white_check_mark: |
| isInSphereRange__15ParallelGravityCFRCQ29JGeometry8TVec3<f>Pf | :white_check_mark: |
| isInBoxRange__15ParallelGravityCFRCQ29JGeometry8TVec3<f>Pf | :x: |
| isInCylinderRange__15ParallelGravityCFRCQ29JGeometry8TVec3<f>Pf | :x: |
| isInRange__15ParallelGravityCFRCQ29JGeometry8TVec3<f>Pf | :white_check_mark: |


# PlanetGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__13PlanetGravityFv | :white_check_mark: |
| setPriority__13PlanetGravityFl | :white_check_mark: |
| calcGravity__13PlanetGravityCFPQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| calcOwnGravityVector__13PlanetGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| isInRangeSquare__13PlanetGravityCFf | :white_check_mark: |
| isInRangeDistance__13PlanetGravityCFf | :white_check_mark: |
| calcGravityFromMassPosition__13PlanetGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| updateMtx__13PlanetGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :white_check_mark: |
| updateIdentityMtx__13PlanetGravityFv | :white_check_mark: |


# PlanetGravityManager.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__20PlanetGravityManagerFPCc | :white_check_mark: |
| init__20PlanetGravityManagerFRC12JMapInfoIter | :white_check_mark: |
| calcTotalGravityVector__20PlanetGravityManagerCFPQ29JGeometry8TVec3<f>P11GravityInfoRCQ29JGeometry8TVec3<f>UlUl | :x: |
| registerGravity__20PlanetGravityManagerFP13PlanetGravity | :x: |
| __dt__20PlanetGravityManagerFv | :white_check_mark: |


# PointGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__12PointGravityFv | :white_check_mark: |
| calcOwnGravityVector__12PointGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| updateMtx__12PointGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :white_check_mark: |


# SegmentGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__14SegmentGravityFv | :white_check_mark: |
| calcOwnGravityVector__14SegmentGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |
| updateMtx__14SegmentGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setGravityPoint__14SegmentGravityFUlRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| setSideVector__14SegmentGravityFRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| setValidSideDegree__14SegmentGravityFf | :white_check_mark: |
| setEdgeValid__14SegmentGravityFUlb | :white_check_mark: |
| updateLocalParam__14SegmentGravityFv | :x: |


