#pragma once

XanimeQuadBckTable quadAnimeTable[7] = {
    {
        "基本", //animationName
        "WalkSoft", //fileName1
        0.00000f, // _8
        "Walk", //fileName2
        0.00000f, // _10
        "Run", //fileName3
        0.00000f, // _18
        "Wait", //fileName4
        1.00000f, // _20
    },
    {
        "水泳基本", //animationName
        "SwimWait", //fileName1
        1.00000f, // _8
        "SwimFlutterSurface", //fileName2
        0.00000f, // _10
        "SwimFlutter", //fileName3
        0.00000f, // _18
        "SwimDrift", //fileName4
        0.00000f, // _20
    },
    {
        "テニス基本", //animationName
        "WalkSoft", //fileName1
        0.00000f, // _8
        "Walk", //fileName2
        0.00000f, // _10
        "TennisRun", //fileName3
        0.00000f, // _18
        "TennisWait", //fileName4
        1.00000f, // _20
    },
    {
        "スライダー尻", //animationName
        "SlideHipForWard", //fileName1
        0.300000f, // _8
        "SlideHipBackward", //fileName2
        0.200000f, // _10
        "SlideHipLeft", //fileName3
        0.250000f, // _18
        "SlideHipRight", //fileName4
        0.250000f, // _20
    },
    {
        "坂すべり下向きあおむけ", //animationName
        "SlideHipForWard", //fileName1
        0.300000f, // _8
        "SlideHipBackward", //fileName2
        0.200000f, // _10
        "SlideHipLeft", //fileName3
        0.250000f, // _18
        "SlideHipRight", //fileName4
        0.250000f, // _20
    },
    {
        "タマコロ移動", //animationName
        "BallIdle", //fileName1
        1.00000f, // _8
        "BallWalkSoft", //fileName2
        0.00000f, // _10
        "BallWalk", //fileName3
        0.00000f, // _18
        "BallRun", //fileName4
        0.00000f, // _20
    },
    {
        "", //animationName
        "", //fileName1
        0.00000f, // _8
        nullptr, //fileName2
        0.00000f, // _10
        nullptr, //fileName3
        0.00000f, // _18
        nullptr, //fileName4
        0.00000f, // _20
    },
};

XanimeTripleBckTable tripleAnimeTable[8] = {
    {
        "坂左右ウエイト", //animationName
        "Wait", //fileName1
        1.00000f, // _8
        "WaitSlopeL", //fileName2
        0.00000f, // _10
        "WaitSlopeR", //fileName3
        0.00000f, // _18
    },
    {
        "坂前後ウエイト", //animationName
        "Wait", //fileName1
        1.00000f, // _8
        "WaitSlopeD", //fileName2
        0.00000f, // _10
        "WaitSlopeU", //fileName3
        0.00000f, // _18
    },
    {
        "サーフィン", //animationName
        "SurfRideLoop", //fileName1
        1.00000f, // _8
        "SurfRideLoopL", //fileName2
        0.00000f, // _10
        "SurfRideLoopR", //fileName3
        0.00000f, // _18
    },
    {
        "サーフィン（加速）", //animationName
        "SurfRideDashLoop", //fileName1
        1.00000f, // _8
        "SurfRideDashLoopL", //fileName2
        0.00000f, // _10
        "SurfRideDashLoopR", //fileName3
        0.00000f, // _18
    },
    {
        "サーフィン傾き開始", //animationName
        "SurfRide", //fileName1
        1.00000f, // _8
        "SurfRideL", //fileName2
        0.00000f, // _10
        "SurfRideR", //fileName3
        0.00000f, // _18
    },
    {
        "サーフィン傾き開始（加速）", //animationName
        "SurfRideDash", //fileName1
        1.00000f, // _8
        "SurfRideDashL", //fileName2
        0.00000f, // _10
        "SurfRideDashR", //fileName3
        0.00000f, // _18
    },
    {
        "サーフィン落下", //animationName
        "SurfFall", //fileName1
        1.00000f, // _8
        "SurfFallL", //fileName2
        0.00000f, // _10
        "SurfFallR", //fileName3
        0.00000f, // _18
    },
    {
        "", //animationName
        "", //fileName1
        0.00000f, // _8
        nullptr, //fileName2
        0.00000f, // _10
        nullptr, //fileName3
        0.00000f, // _18
    },
};

XanimeDoubleBckTable doubleAnimeTable[4] = {
    {
        "しゃがみ基本", //animationName
        "SquatWait", //fileName1
        1.00000f, // _8
        "SquatWalk", //fileName2
        0.00000f, // _10
    },
    {
        "その場足踏み", //animationName
        "Run", //fileName1
        0.750000f, // _8
        "Wait", //fileName2
        0.250000f, // _10
    },
    {
        "タマコロしゃがみ", //animationName
        "BallSquat", //fileName1
        1.00000f, // _8
        "BallWalk", //fileName2
        0.00000f, // _10
    },
    {
        "", //animationName
        "", //fileName1
        0.00000f, // _8
        nullptr, //fileName2
        0.00000f, // _10
    },
};

XanimeSingleBckTable singleAnimeTable[0x124] = {
    {
        "ジャンプ", //animationName
        "Jump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ジャンプB", //animationName
        "Jump2", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ジャンプC", //animationName
        "JumpRoll", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ターンジャンプ", //animationName
        "JumpTurn", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "空中一回転", //animationName
        "AirControl", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "フリージャンプ", //animationName
        "Rise", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ヘリコプタージャンプ", //animationName
        "FlickAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ダッシュジャンプ", //animationName
        "Rolling", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "飛びすさりジャンプ", //animationName
        "Bounce", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ショートジャンプ", //animationName
        "GravityChange", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スカイラブジャンプ", //animationName
        "JumpTwin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スケキヨ", //animationName
        "Bury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スケキヨ脱出", //animationName
        "Bury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "埋まり", //animationName
        "BuryStandWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "埋まり脱出", //animationName
        "BuryStandEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "逆着地", //animationName
        "CannonFlyLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパージャンプA", //animationName
        "HopperWaitA", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパージャンプB", //animationName
        "HopperWaitB", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパーふみジャンプA", //animationName
        "HopperWaitA", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパーふみジャンプB", //animationName
        "HopperWaitB", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパー移動A", //animationName
        "HopperRunA", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパー移動B", //animationName
        "HopperRunB", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパーハイジャンプA", //animationName
        "HopperJumpA", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパーハイジャンプB", //animationName
        "HopperJumpB", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパー壁ジャンプ", //animationName
        "HopperWallJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパーヒップドロップ開始", //animationName
        "HopperMarioHipDropStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ホッパーヒップドロップ", //animationName
        "HopperMarioHipDrop", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ジャンプふみ1", //animationName
        "Jump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ジャンプふみ2", //animationName
        "JumpPress2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ジャンプふみ3", //animationName
        "JumpPress3rd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "引き戻し", //animationName
        "PullBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "引き戻し着地", //animationName
        "PullBackLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ポッドワープ開始", //animationName
        "WarpPodStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ポッドワープ終了", //animationName
        "WarpPodEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "幅とび", //animationName
        "JumpBroad", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "着地", //animationName
        "Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "着地B", //animationName
        "Jump2Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "着地C", //animationName
        "JumpRollLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "着地ターン", //animationName
        "JumpTurnLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "着地幅とび", //animationName
        "JumpBroadLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハード着地", //animationName
        "LandStiffen", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "すべり着地", //animationName
        "LandSlope", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ショート着地", //animationName
        "Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ヒップドロップ開始", //animationName
        "HipDropStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ヒップドロップ", //animationName
        "HipDrop", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ヒップドロップ着地", //animationName
        "HipDropLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ヒップドロップ滑り", //animationName
        "LandRotation", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ジャンプ順滑り", //animationName
        "LandRotation", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ジャンプ逆滑り", //animationName
        "Fall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンヒップドロップ開始", //animationName
        "HipDropHomingStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンヒップドロップ", //animationName
        "HipDropHoming", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンヒップドロップ着地", //animationName
        "HipDropHomingLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スリップアップ", //animationName
        "SlipUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "つかまりスリップアップ準備", //animationName
        "HangSlipUpStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "つかまりスリップアップ", //animationName
        "HangSlipUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ飛行中", //animationName
        "BeeFly", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ飛行中無入力", //animationName
        "BeeFlyWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ壁くっつき", //animationName
        "BeeLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ壁くっつき中", //animationName
        "BeeWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ花移動", //animationName
        "BeeCreepWallWalk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチジャンプ", //animationName
        "BeeJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ壁ジャンプ", //animationName
        "BeeWallJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ匍匐ウエイト", //animationName
        "BeeCreepWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ匍匐前進", //animationName
        "BeeCreepWalk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチ匍匐着地", //animationName
        "BeeCreepLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチヒップドロップ開始", //animationName
        "BeeHipDropStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチヒップドロップ", //animationName
        "BeeHipDrop", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチヒップドロップ着地", //animationName
        "BeeHipDropLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチヒップドロップ壁着地", //animationName
        "BeeHipDropLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "泥低速歩行", //animationName
        "WalkSoft", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "泥高速歩行", //animationName
        "WalkBury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "落下", //animationName
        "Fall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "フーファイター飛行開始", //animationName
        "FooStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "フーファイター飛行", //animationName
        "FooFly", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "フーファイター飛行再開", //animationName
        "FooFlyStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "フーファイター静止", //animationName
        "FooWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "フーファイター解除", //animationName
        "FooEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "フーファイター着地", //animationName
        "LandSlope", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "フーファイタースピン", //animationName
        "FooSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁ジャンプ", //animationName
        "WallJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁すべり", //animationName
        "WallSlide", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁くっつき", //animationName
        "WallKeep", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁押し", //animationName
        "Push", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁左歩き", //animationName
        "WallWalkL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁右歩き", //animationName
        "WallWalkR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁ウエイト", //animationName
        "WallWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "前壁ウエイト", //animationName
        "Push", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁ヒット", //animationName
        "WallHit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁ヒット着地", //animationName
        "WallHitLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "壁はじき", //animationName
        "WallHit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "崖ふんばり", //animationName
        "Stagger", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "カリカリ限界", //animationName
        "WaitHold", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "坂左ウエイト", //animationName
        "WaitSlopeL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "坂右ウエイト", //animationName
        "WaitSlopeR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "坂前ウエイト", //animationName
        "WaitSlopeD", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "坂後ウエイト", //animationName
        "WaitSlopeU", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ラン", //animationName
        "Run", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "歩行", //animationName
        "Walk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "鈍行", //animationName
        "WalkSoft", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "メタルダッシュ", //animationName
        "RunDash", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "埋まり歩行", //animationName
        "WalkBury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "埋まりジャンプA", //animationName
        "WalkBuryJumpLow", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "埋まりジャンプB", //animationName
        "WalkBuryJumpLow2", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "埋まり脱出ジャンプ", //animationName
        "WalkBuryJumpHi", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "つぶれ", //animationName
        "Press", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "つぶれ復帰", //animationName
        "PressRecover", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "つぶれ解除", //animationName
        "Jump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ブレーキ", //animationName
        "Brake", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ブレーキ滑り床", //animationName
        "Run", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ターンブレーキ", //animationName
        "Turn", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ターンブレーキ滑り床", //animationName
        "Run", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "歩行制動ブレーキ", //animationName
        "RunEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷上慣性走行", //animationName
        "Brake", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷上力行左", //animationName
        "SkateL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷上力行右", //animationName
        "SkateR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷上切替左", //animationName
        "SkateSwitchL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷上切替右", //animationName
        "SkateSwitchR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷上後行左", //animationName
        "SkateBackL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷上後行右", //animationName
        "SkateBackR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷上ターン", //animationName
        "SkateTurn", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しゃがみ", //animationName
        "SquatWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しゃがみ開始", //animationName
        "SquatStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しゃがみ終了", //animationName
        "SquatEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "坂すべり上向きうつぶせ", //animationName
        "SlideStmach", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "坂すべり上向き終了", //animationName
        "SlideStmachEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "坂すべり下向き終了", //animationName
        "SlideHipEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "特殊ウエイト1A", //animationName
        "Sleep", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "特殊ウエイト1B", //animationName
        "SleepLie", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "戦闘ウエイト", //animationName
        "BattleWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "空転", //animationName
        "Run", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ひろい", //animationName
        "CarryStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ひろい空中", //animationName
        "CarryAirStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ひろいクイック", //animationName
        "CarryStartShort", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ひろいウエイト", //animationName
        "CarryWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "カブ抜き", //animationName
        "PullOut", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "カブウエイト", //animationName
        "PullOutWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハンマー投げ回転中", //animationName
        "Swing", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハンマー投げリリース", //animationName
        "SwingThrow", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "コインゲット", //animationName
        "CoinGet", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "パンチ", //animationName
        "Spin2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "空パンチ", //animationName
        "Spin2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "左パンチ", //animationName
        "Spin2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "左空パンチ", //animationName
        "Spin2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ランニングキック", //animationName
        "Kick", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "アッパーパンチ", //animationName
        "Jump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しゃがみアッパー", //animationName
        "SquatEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "サマーソルト", //animationName
        "SpinLow", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "投げ", //animationName
        "Throw", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ファイア投げ", //animationName
        "Throw", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "両手投げ", //animationName
        "ThrowBoth", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ファイアスピン", //animationName
        "FireSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ファイアスピン空中", //animationName
        "FireSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "崖つかまり開始", //animationName
        "HangStartUnder", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "崖つかまり中", //animationName
        "HangWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "崖つかまり終了", //animationName
        "HangUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "崖つかまり終了坂", //animationName
        "HangUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "崖つかまり降り", //animationName
        "HangStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しゃがみジャンプ", //animationName
        "JumpBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しゃがみジャンプ着地", //animationName
        "JumpBackLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しゃがみ歩き", //animationName
        "SquatWalk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "がんばり走り", //animationName
        "RunSlope", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "腹ばいジャンプ", //animationName
        "SlideStomachRecover", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "尻滑りジャンプ", //animationName
        "SlideHipRecover", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "向かい風走り", //animationName
        "RunSlope", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "向かい風ふんばり", //animationName
        "Stagger", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "その場足踏み上半身", //animationName
        "Walk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ウエイト", //animationName
        "SwimWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳一掻き", //animationName
        "SwimBreast", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水上一掻き", //animationName
        "SwimBreastSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳潜り", //animationName
        "SwimDive", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ジャンプダイブ", //animationName
        "LandWater", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ジャンプダイブ回転", //animationName
        "LandWaterDive", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳上昇呼吸", //animationName
        "SwimRise", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳水面初期移動", //animationName
        "SwimStartSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水上スピン", //animationName
        "SwimSpinSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳スピン", //animationName
        "SwimSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳スピン移動", //animationName
        "SwimSpinAttack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水上スピン移動", //animationName
        "SwimSpinAttackSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳スピンジャンプ", //animationName
        "SwimJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ダメージ", //animationName
        "SwimDamageSmall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ダメージ亀", //animationName
        "SwimFlutterBoardDamageSmall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ダメージ中", //animationName
        "SwimDamageMiddle", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水上ダメージ中", //animationName
        "SwimDamageMiddleSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水上ダメージ着水", //animationName
        "SwimDamageMiddleSurfaceLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ジェット", //animationName
        "SwimFlutterboard", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ジェット終了", //animationName
        "SwimDamageMiddle", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳亀投げ", //animationName
        "SwimFlutterboardThrow", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ジェット開始", //animationName
        "SwimFlutterboardStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ジェット壁ターン", //animationName
        "SwimFlutterboardTurn", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ダウン", //animationName
        "SwimDie", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ターン左", //animationName
        "SwimTurnL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ターン右", //animationName
        "SwimTurnR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳壁ヒット", //animationName
        "SwimWallHit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳ターン下", //animationName
        "SwimTurnForward", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水泳陸うちあげ", //animationName
        "SlideStmachEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "飛び込み準備", //animationName
        "DiveWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "飛び込みジャンプ", //animationName
        "Dive", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "後方飛び込みジャンプ", //animationName
        "DiveBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "飛び込み失敗着地", //animationName
        "Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "飛び込み失敗回転着地", //animationName
        "LandRotation", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "リングダッシュ", //animationName
        "SwimDashRing", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "リングダッシュ準備", //animationName
        "SwimDashRingStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "カメ持ちリング準備", //animationName
        "SwimFlutterBoardDashRingStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "カメ持ちリング", //animationName
        "SwimFlutterBoardDashRing", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "匍匐前進", //animationName
        "SwimGetUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "前方小ダメージ", //animationName
        "DamageSmallFront", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "後方小ダメージ", //animationName
        "DamageSmallBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "強制ジャンプ", //animationName
        "Rise", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "中ダメージ", //animationName
        "DamageMiddleFront", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "中ダメージ空中", //animationName
        "DamageMiddleFrontAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "中ダメージ着地", //animationName
        "DamageMiddleFrontLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "中後ダメージ", //animationName
        "DamageMiddleBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "中後ダメージ空中", //animationName
        "DamageMiddleBackAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "中後ダメージ着地", //animationName
        "DamageMiddleBackLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "炎ダメージ", //animationName
        "DamageFire", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ノーダメージ", //animationName
        "DamageBit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "炎のランナー", //animationName
        "FireRun", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ファイアラン前兆", //animationName
        "FireRunStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "電気ダメージ", //animationName
        "DamageElectric", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "電気ダメージ終了", //animationName
        "DamageElectricEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ファイアダンス", //animationName
        "FireRun", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ダメージ", //animationName
        "DamageStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ダメージ着地", //animationName
        "Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ダメージウエイト", //animationName
        "DamageWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ノーマルウエイト", //animationName
        "Wait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷結", //animationName
        "Freeze", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "氷結解除", //animationName
        "IceFlick", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しびれ", //animationName
        "DamageNumb", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "しびれ回復", //animationName
        "DamageNumbEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "はねとばされ", //animationName
        "DamageFlick", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "はねとばされ終了", //animationName
        "DamageFlickEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "後転ふっとび", //animationName
        "DamageWeakFront", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "後転ふっとび空中", //animationName
        "DamageWeakFrontAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "後転ふっとび着地", //animationName
        "DamageWeakFrontLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "座りダウン", //animationName
        "DieSit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "感電ダウン", //animationName
        "DieElectric", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "炎ダウン", //animationName
        "DieFire", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "仰向けダウン", //animationName
        "DieOver", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "俯せダウン", //animationName
        "DieUnder", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "奈落ダウン", //animationName
        "DieFall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ブラックホール落下", //animationName
        "DieBlackHole", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "つぶれダウン", //animationName
        "DieSit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ゲームオーバー", //animationName
        "DieOver", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "レース負け", //animationName
        "DieEvent", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "水中レース負け", //animationName
        "DieSwimEvent", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "埋まりダウン", //animationName
        "DieBury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "テニスショット左", //animationName
        "TennisShotL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "テニスショット右", //animationName
        "TennisShotR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "テニスショット中", //animationName
        "TennisShotM", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "テニスショット空", //animationName
        "TennisShotAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "テニスウエイト", //animationName
        "TennisWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "エレメントゲット", //animationName
        "ElementGet", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "エレメントゲット接地中", //animationName
        "ElementGetGround", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "空中ひねり", //animationName
        "Spin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "地上ひねり", //animationName
        "SpinGround", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "アイスひねり", //animationName
        "IceSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "アイスひねり移動", //animationName
        "IceSkateSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "アイスひねり静止", //animationName
        "IceSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチスピン空中", //animationName
        "BeeSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ハチスピン", //animationName
        "BeeSpinGround", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "アイスひねり空中", //animationName
        "IceSpinAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スケートアクセルジャンプ", //animationName
        "IceJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スケートジャンプ2", //animationName
        "IceJump2", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スケートジャンプ3", //animationName
        "IceJump3", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スケート着地", //animationName
        "IceJumpLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スケート静止着地", //animationName
        "IceJumpStopLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "サーフィンジャンプ", //animationName
        "SurfJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "サーフィンハイジャンプ", //animationName
        "SurfJumpHigh", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "サーフィン着地", //animationName
        "SurfLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "見る", //animationName
        "Watch", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "レース見る", //animationName
        "Watch", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ステージインA", //animationName
        "StageStartGround", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ステージインB", //animationName
        "LandScenarioStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ウォークイン", //animationName
        "GoThrough", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[デモ1]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[会話1]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[デモ2]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[会話2]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[デモ3]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[会話3]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[デモ4]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[デモ5]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[会話4]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[デモ6]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[デモ7]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "スピンゲット[デモ8]", //animationName
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "レース開始", //animationName
        "RaceStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "レースクラウチング開始", //animationName
        "RaceStartCrouch", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ゴーストレース開始", //animationName
        "RaceStartGhost", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ゴースト勝利", //animationName
        "WinGhost", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "ゴースト出現", //animationName
        "AppearGhost", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        "", //animationName
        "", //fileName
        0, // animationHash
        0, // fileHash
    },
};

XanimeGroupInfo marioAnimeTable[0x134] = {
    {
        "基本", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しゃがみ基本", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳基本", // _0
        1.00000f, // _4
        0x1e, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "テニス基本", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂左右ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂前後ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂左ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂右ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂前ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂後ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ラン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "歩行", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "鈍行", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "メタルダッシュ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ダッシュジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "飛びすさりジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ショートジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "埋まり歩行", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "埋まりジャンプA", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "埋まりジャンプB", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "埋まり脱出ジャンプ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "逆着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しゃがみ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ブレーキ", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ブレーキ滑り床", // _0
        4.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ターンブレーキ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ターンブレーキ滑り床", // _0
        3.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "歩行制動ブレーキ", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷上慣性走行", // _0
        1.00000f, // _4
        0x10, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷上力行左", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷上力行右", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷上切替左", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷上切替右", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷上後行左", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷上後行右", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷上ターン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "特殊ウエイト1A", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "特殊ウエイト1B", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "戦闘ウエイト", // _0
        1.00000f, // _4
        0x1e, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "匍匐前進", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ジャンプ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ジャンプB", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ジャンプC", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ターンジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "フリージャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ジャンプふみ1", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ジャンプふみ2", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ジャンプふみ3", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "引き戻し", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "引き戻し着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ポッドワープ開始", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ポッドワープ終了", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スカイラブジャンプ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "空中一回転", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スケキヨ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スケキヨ脱出", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "埋まり", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "埋まり脱出", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパージャンプA", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパージャンプB", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパーふみジャンプA", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパーふみジャンプB", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパー移動A", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパー移動B", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパーハイジャンプA", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパーハイジャンプB", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパー壁ジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパーヒップドロップ開始", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ホッパーヒップドロップ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "幅とび", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ヒップドロップ開始", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ヒップドロップ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ヒップドロップ着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ヒップドロップ滑り", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ジャンプ順滑り", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ジャンプ逆滑り", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スリップアップ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "つかまりスリップアップ準備", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "つかまりスリップアップ", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンヒップドロップ開始", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンヒップドロップ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンヒップドロップ着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "落下", // _0
        1.00000f, // _4
        0xf, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "着地", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "着地B", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハード着地", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "すべり着地", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ショート着地", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ飛行中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ飛行中無入力", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ壁くっつき", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ壁くっつき中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ壁ジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ匍匐ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ匍匐前進", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ匍匐着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチ花移動", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチヒップドロップ開始", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチヒップドロップ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチヒップドロップ着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチヒップドロップ壁着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "フーファイター飛行開始", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "フーファイター飛行", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "フーファイター飛行再開", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "フーファイター静止", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "フーファイター解除", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "フーファイター着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "フーファイタースピン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "泥低速歩行", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "泥高速歩行", // _0
        2.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁くっつき", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁すべり", // _0
        1.00000f, // _4
        0xa, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁ジャンプ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁押し", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁左歩き", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁右歩き", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "前壁ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁ヒット", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁ヒット着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "壁はじき", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ダメージ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ダメージ着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ダメージウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ノーマルウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷結", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "氷結解除", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しびれ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しびれ回復", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "コインゲット", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "パンチ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "空パンチ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "左パンチ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "左空パンチ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ランニングキック", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "アッパーパンチ", // _0
        1.00000f, // _4
        0x3, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しゃがみアッパー", // _0
        1.00000f, // _4
        0x3, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サマーソルト", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ゲームオーバー", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "つぶれ", // _0
        0.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "つぶれ復帰", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "つぶれ解除", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "空転", // _0
        4.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂すべり上向きうつぶせ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂すべり下向きあおむけ", // _0
        2.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "向かい風走り", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "向かい風ふんばり", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サーフィン", // _0
        1.00000f, // _4
        0xf, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サーフィン（加速）", // _0
        1.00000f, // _4
        0xf, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サーフィン傾き開始", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サーフィン傾き開始（加速）", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サーフィンジャンプ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サーフィンハイジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サーフィン落下", // _0
        1.00000f, // _4
        0xf, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "サーフィン着地", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "タマコロ移動", // _0
        1.00000f, // _4
        0xf, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "タマコロしゃがみ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スライダー尻", // _0
        2.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "その場足踏み", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "その場足踏み上半身", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳一掻き", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水上一掻き", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳潜り", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ジャンプダイブ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ジャンプダイブ回転", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳上昇呼吸", // _0
        1.00000f, // _4
        0x10, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳水面初期移動", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水上スピン", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳スピン", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳スピン移動", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水上スピン移動", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ダメージ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ダメージ亀", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ダメージ中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水上ダメージ中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水上ダメージ着水", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ダウン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ターン左", // _0
        1.00000f, // _4
        0x14, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ターン右", // _0
        1.00000f, // _4
        0x14, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ターン下", // _0
        1.00000f, // _4
        0x14, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ジェット", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ジェット開始", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ジェット終了", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳亀投げ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳壁ヒット", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳ジェット壁ターン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳陸うちあげ", // _0
        1.00000f, // _4
        0x10, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "飛び込み準備", // _0
        1.00000f, // _4
        0x14, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "飛び込みジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "後方飛び込みジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "飛び込み失敗着地", // _0
        1.00000f, // _4
        0xf, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "飛び込み失敗回転着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水泳スピンジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "リングダッシュ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "リングダッシュ準備", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "カメ持ちリング準備", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "カメ持ちリング", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "着地C", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "着地ターン", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "着地幅とび", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しゃがみ開始", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しゃがみ終了", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂すべり上向き終了", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "坂すべり下向き終了", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ひろい", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ひろい空中", // _0
        1.00000f, // _4
        0x3, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ひろいクイック", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ひろいウエイト", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "カブ抜き", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "カブウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハンマー投げ回転中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハンマー投げリリース", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "投げ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ファイア投げ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "両手投げ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ファイアスピン", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ファイアスピン空中", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "崖つかまり開始", // _0
        1.00000f, // _4
        0x5, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "崖つかまり中", // _0
        1.00000f, // _4
        0x5, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "崖つかまり終了", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "崖つかまり終了坂", // _0
        1.00000f, // _4
        0x4, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "崖つかまり降り", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "崖ふんばり", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しゃがみジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しゃがみジャンプ着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "しゃがみ歩き", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "がんばり走り", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "腹ばいジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "尻滑りジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "前方小ダメージ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "後方小ダメージ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "強制ジャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ヘリコプタージャンプ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "中ダメージ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "中ダメージ空中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "中ダメージ着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "中後ダメージ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "中後ダメージ空中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "中後ダメージ着地", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ノーダメージ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "炎ダメージ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "炎のランナー", // _0
        8.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ファイアラン前兆", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "電気ダメージ", // _0
        1.00000f, // _4
        0x1, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "電気ダメージ終了", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ファイアダンス", // _0
        6.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "後転ふっとび", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "後転ふっとび空中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "後転ふっとび着地", // _0
        1.00000f, // _4
        0x2, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "座りダウン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "感電ダウン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "炎ダウン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "仰向けダウン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "俯せダウン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "奈落ダウン", // _0
        1.00000f, // _4
        0x14, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ブラックホール落下", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "レース負け", // _0
        1.00000f, // _4
        0xf, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "水中レース負け", // _0
        1.00000f, // _4
        0xf, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "埋まりダウン", // _0
        1.00000f, // _4
        0x1e, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "つぶれダウン", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "はねとばされ", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "はねとばされ終了", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "テニスショット左", // _0
        1.25000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "テニスショット右", // _0
        1.25000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "テニスショット中", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "テニスショット空", // _0
        1.25000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "テニスウエイト", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "エレメントゲット", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "エレメントゲット接地中", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "空中ひねり", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "地上ひねり", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチスピン", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ハチスピン空中", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "アイスひねり", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "アイスひねり移動", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "アイスひねり静止", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "アイスひねり空中", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スケートアクセルジャンプ", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スケートジャンプ2", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スケートジャンプ3", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スケート着地", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スケート静止着地", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "見る", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "レース見る", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "カリカリ限界", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ステージインA", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ステージインB", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ウォークイン", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[デモ1]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[会話1]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[デモ2]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[会話2]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[デモ3]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[会話3]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[デモ4]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[デモ5]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[会話4]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[デモ6]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[デモ7]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "スピンゲット[デモ8]", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "レース開始", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "レースクラウチング開始", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ゴーストレース開始", // _0
        1.00000f, // _4
        0x6, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ゴースト勝利", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "ゴースト出現", // _0
        1.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    },
    {
        "", // _0
        0.00000f, // _4
        0, // _8
        0.00000f, // _C
        0.00000f, // _10
        0.00000f, // _14
        0, // _18
        0, // _1C
        0, // _1D
        {
            0,
            0,
            0,
            0,
        }, // _20
        {
            0.00000f,
            0.00000f,
            0.00000f,
            0.00000f,
        }, // _30
        0, // _40
        0, // _44
        nullptr, // _48
    }
};

XanimeAuxInfo marioAnimeAuxTable[1] = {
    {""}
};

XanimeOfsInfo marioAnimeOfsTable[35] = {
    {
        "ダメージ", //animationName
        0.00000f, // _4
        10.0000f, // _8
        10.0000f, // _C
        0x1, // _10
    },
    {
        "坂すべり下向きあおむけ", //animationName
        0.00000f, // _4
        2.00000f, // _8
        2.00000f, // _C
        0x1, // _10
    },
    {
        "坂すべり上向きうつぶせ", //animationName
        0.00000f, // _4
        2.00000f, // _8
        2.00000f, // _C
        0x1, // _10
    },
    {
        "ターンブレーキ", //animationName
        0.00000f, // _4
        20.0000f, // _8
        20.0000f, // _C
        0x1, // _10
    },
    {
        "ジャンプ", //animationName
        0.00000f, // _4
        24.0000f, // _8
        24.0000f, // _C
        0x1, // _10
    },
    {
        "壁ヒット", //animationName
        0.00000f, // _4
        25.0000f, // _8
        25.0000f, // _C
        0x1, // _10
    },
    {
        "壁ヒット着地", //animationName
        0.00000f, // _4
        67.0000f, // _8
        0.00000f, // _C
        0, // _10
    },
    {
        "カブ抜き", //animationName
        0.00000f, // _4
        49.0000f, // _8
        49.0000f, // _C
        0x1, // _10
    },
    {
        "落下", //animationName
        0.00000f, // _4
        29.0000f, // _8
        29.0000f, // _C
        0x1, // _10
    },
    {
        "ヒップドロップ開始", //animationName
        0.00000f, // _4
        13.0000f, // _8
        13.0000f, // _C
        0x1, // _10
    },
    {
        "ヒップドロップ", //animationName
        0.00000f, // _4
        9.00000f, // _8
        9.00000f, // _C
        0x1, // _10
    },
    {
        "スピンヒップドロップ", //animationName
        0.00000f, // _4
        19.0000f, // _8
        19.0000f, // _C
        0x1, // _10
    },
    {
        "ハチヒップドロップ壁着地", //animationName
        0.00000f, // _4
        20.0000f, // _8
        20.0000f, // _C
        0, // _10
    },
    {
        "ホッパーふみジャンプA", //animationName
        10.0000f, // _4
        50.0000f, // _8
        0.00000f, // _C
        0x1, // _10
    },
    {
        "ホッパーふみジャンプB", //animationName
        10.0000f, // _4
        50.0000f, // _8
        0.00000f, // _C
        0x1, // _10
    },
    {
        "テニスショット左", //animationName
        4.00000f, // _4
        26.0000f, // _8
        26.0000f, // _C
        0, // _10
    },
    {
        "テニスショット右", //animationName
        4.00000f, // _4
        26.0000f, // _8
        26.0000f, // _C
        0, // _10
    },
    {
        "テニスショット中", //animationName
        3.00000f, // _4
        38.0000f, // _8
        38.0000f, // _C
        0, // _10
    },
    {
        "テニスショット空", //animationName
        5.00000f, // _4
        26.0000f, // _8
        26.0000f, // _C
        0, // _10
    },
    {
        "その場足踏み", //animationName
        0.00000f, // _4
        17.0000f, // _8
        8.00000f, // _C
        0, // _10
    },
    {
        "ブレーキ", //animationName
        0.00000f, // _4
        2.00000f, // _8
        0.00000f, // _C
        0x1, // _10
    },
    {
        "ショート着地", //animationName
        0.00000f, // _4
        5.00000f, // _8
        0.00000f, // _C
        0, // _10
    },
    {
        "アッパーパンチ", //animationName
        22.0000f, // _4
        23.0000f, // _8
        0.00000f, // _C
        0x1, // _10
    },
    {
        "しゃがみアッパー", //animationName
        0.00000f, // _4
        6.00000f, // _8
        0.00000f, // _C
        0, // _10
    },
    {
        "スケキヨ", //animationName
        0.00000f, // _4
        59.0000f, // _8
        30.0000f, // _C
        0x2, // _10
    },
    {
        "スケキヨ脱出", //animationName
        60.0000f, // _4
        120.000f, // _8
        0.00000f, // _C
        0, // _10
    },
    {
        "特殊ウエイト1A", //animationName
        0.00000f, // _4
        494.000f, // _8
        375.000f, // _C
        0x2, // _10
    },
    {
        "特殊ウエイト1B", //animationName
        0.00000f, // _4
        254.000f, // _8
        135.000f, // _C
        0x2, // _10
    },
    {
        "アイスひねり", //animationName
        0.00000f, // _4
        35.0000f, // _8
        35.0000f, // _C
        0, // _10
    },
    {
        "ファイアスピン空中", //animationName
        0.00000f, // _4
        30.0000f, // _8
        30.0000f, // _C
        0, // _10
    },
    {
        "崖つかまり終了坂", //animationName
        0.00000f, // _4
        27.0000f, // _8
        27.0000f, // _C
        0, // _10
    },
    {
        "前壁ウエイト", //animationName
        57.0000f, // _4
        59.0000f, // _8
        57.0000f, // _C
        0x1, // _10
    },
    {
        "フーファイター静止", //animationName
        0.00000f, // _4
        159.000f, // _8
        40.0000f, // _C
        0x2, // _10
    },
    {
        "ひろいクイック", //animationName
        0.00000f, // _4
        26.0000f, // _8
        0.00000f, // _C
        0x1, // _10
    },
    {
        "", //animationName
        0.00000f, // _4
        0.00000f, // _8
        0.00000f, // _C
        0, // _10
    },
};

XanimeSwapTable luigiAnimeSwapTable[14] = {
    {"Run", "LuigiRun"},
    {"Jump", "LuigiJump"},
    {"JumpRoll", "LuigiJumpRoll"},
    {"JumpBack", "LuigiJumpBack"},
    {"RunEnd", "LuigiRunEnd"},
    {"Spin", "LuigiSpin"},
    {"SpinGround", "LuigiSpinGround"},
    {"SpaceFlyShort", "LuigiSpaceFlyShort"},
    {"Wait", "LuigiWait"},
    {"WaitSlopeL", "LuigiWaitSlopeL"},
    {"WaitSlopeR", "LuigiWaitSlopeR"},
    {"WaitSlopeU", "LuigiWaitSlopeU"},
    {"WaitSlopeD", "LuigiWaitSlopeD"},
    {"", nullptr},
};
