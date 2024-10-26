#pragma once

#include "Game/Animation/XanimeResource.hpp"

XanimeQuadBckTable quadAnimeTable[7] = {
    {
        {"��{"}, //mParent
        "WalkSoft", //fileName1
        0.00000000000f, // 0x8
        "Walk", //fileName2
        0.00000000000f, // 0x10
        "Run", //fileName3
        0.00000000000f, // 0x18
        "Wait", //fileName4
        1.00000000000f, // 0x20
    },
    {
        {"���j��{"}, //mParent
        "SwimWait", //fileName1
        1.00000000000f, // 0x8
        "SwimFlutterSurface", //fileName2
        0.00000000000f, // 0x10
        "SwimFlutter", //fileName3
        0.00000000000f, // 0x18
        "SwimDrift", //fileName4
        0.00000000000f, // 0x20
    },
    {
        {"�e�j�X��{"}, //mParent
        "WalkSoft", //fileName1
        0.00000000000f, // 0x8
        "Walk", //fileName2
        0.00000000000f, // 0x10
        "TennisRun", //fileName3
        0.00000000000f, // 0x18
        "TennisWait", //fileName4
        1.00000000000f, // 0x20
    },
    {
        {"�X���C�_�[�K"}, //mParent
        "SlideHipForWard", //fileName1
        0.300000011921f, // 0x8
        "SlideHipBackward", //fileName2
        0.200000002980f, // 0x10
        "SlideHipLeft", //fileName3
        0.250000000000f, // 0x18
        "SlideHipRight", //fileName4
        0.250000000000f, // 0x20
    },
    {
        {"�₷�ׂ艺���������ނ�"}, //mParent
        "SlideHipForWard", //fileName1
        0.300000011921f, // 0x8
        "SlideHipBackward", //fileName2
        0.200000002980f, // 0x10
        "SlideHipLeft", //fileName3
        0.250000000000f, // 0x18
        "SlideHipRight", //fileName4
        0.250000000000f, // 0x20
    },
    {
        {"�^�}�R���ړ�"}, //mParent
        "BallIdle", //fileName1
        1.00000000000f, // 0x8
        "BallWalkSoft", //fileName2
        0.00000000000f, // 0x10
        "BallWalk", //fileName3
        0.00000000000f, // 0x18
        "BallRun", //fileName4
        0.00000000000f, // 0x20
    },
    {
        {""}, //mParent
        "", //fileName1
        0.00000000000f, // 0x8
        nullptr, //fileName2
        0.00000000000f, // 0x10
        nullptr, //fileName3
        0.00000000000f, // 0x18
        nullptr, //fileName4
        0.00000000000f, // 0x20
    },
};

XanimeTripleBckTable tripleAnimeTable[8] = {
    {
        {"�⍶�E�E�G�C�g"}, //mParent
        "Wait", //fileName1
        1.00000000000f, // 0x8
        "WaitSlopeL", //fileName2
        0.00000000000f, // 0x10
        "WaitSlopeR", //fileName3
        0.00000000000f, // 0x18
    },
    {
        {"��O��E�G�C�g"}, //mParent
        "Wait", //fileName1
        1.00000000000f, // 0x8
        "WaitSlopeD", //fileName2
        0.00000000000f, // 0x10
        "WaitSlopeU", //fileName3
        0.00000000000f, // 0x18
    },
    {
        {"�T�[�t�B��"}, //mParent
        "SurfRideLoop", //fileName1
        1.00000000000f, // 0x8
        "SurfRideLoopL", //fileName2
        0.00000000000f, // 0x10
        "SurfRideLoopR", //fileName3
        0.00000000000f, // 0x18
    },
    {
        {"�T�[�t�B���i�����j"}, //mParent
        "SurfRideDashLoop", //fileName1
        1.00000000000f, // 0x8
        "SurfRideDashLoopL", //fileName2
        0.00000000000f, // 0x10
        "SurfRideDashLoopR", //fileName3
        0.00000000000f, // 0x18
    },
    {
        {"�T�[�t�B���X���J�n"}, //mParent
        "SurfRide", //fileName1
        1.00000000000f, // 0x8
        "SurfRideL", //fileName2
        0.00000000000f, // 0x10
        "SurfRideR", //fileName3
        0.00000000000f, // 0x18
    },
    {
        {"�T�[�t�B���X���J�n�i�����j"}, //mParent
        "SurfRideDash", //fileName1
        1.00000000000f, // 0x8
        "SurfRideDashL", //fileName2
        0.00000000000f, // 0x10
        "SurfRideDashR", //fileName3
        0.00000000000f, // 0x18
    },
    {
        {"�T�[�t�B������"}, //mParent
        "SurfFall", //fileName1
        1.00000000000f, // 0x8
        "SurfFallL", //fileName2
        0.00000000000f, // 0x10
        "SurfFallR", //fileName3
        0.00000000000f, // 0x18
    },
    {
        {""}, //mParent
        "", //fileName1
        0.00000000000f, // 0x8
        nullptr, //fileName2
        0.00000000000f, // 0x10
        nullptr, //fileName3
        0.00000000000f, // 0x18
    },
};

XanimeDoubleBckTable doubleAnimeTable[4] = {
    {
        {"���Ⴊ�݊�{"}, //mParent
        "SquatWait", //fileName1
        1.00000000000f, // 0x8
        "SquatWalk", //fileName2
        0.00000000000f, // 0x10
    },
    {
        {"���̏ꑫ����"}, //mParent
        "Run", //fileName1
        0.750000000000f, // 0x8
        "Wait", //fileName2
        0.250000000000f, // 0x10
    },
    {
        {"�^�}�R�����Ⴊ��"}, //mParent
        "BallSquat", //fileName1
        1.00000000000f, // 0x8
        "BallWalk", //fileName2
        0.00000000000f, // 0x10
    },
    {
        {""}, //mParent
        "", //fileName1
        0.00000000000f, // 0x8
        nullptr, //fileName2
        0.00000000000f, // 0x10
    },
};

XanimeSingleBckTable singleAnimeTable[0x124] = {
    {
        {"�W�����v"}, //mParent
        "Jump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�W�����vB"}, //mParent
        "Jump2", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�W�����vC"}, //mParent
        "JumpRoll", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�^�[���W�����v"}, //mParent
        "JumpTurn", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�󒆈��]"}, //mParent
        "AirControl", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t���[�W�����v"}, //mParent
        "Rise", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�w���R�v�^�[�W�����v"}, //mParent
        "FlickAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�_�b�V���W�����v"}, //mParent
        "Rolling", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��т�����W�����v"}, //mParent
        "Bounce", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�V���[�g�W�����v"}, //mParent
        "GravityChange", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�J�C���u�W�����v"}, //mParent
        "JumpTwin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�P�L��"}, //mParent
        "Bury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�P�L���E�o"}, //mParent
        "Bury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂�"}, //mParent
        "BuryStandWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂�E�o"}, //mParent
        "BuryStandEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t���n"}, //mParent
        "CannonFlyLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�W�����vA"}, //mParent
        "HopperWaitA", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�W�����vB"}, //mParent
        "HopperWaitB", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�ӂ݃W�����vA"}, //mParent
        "HopperWaitA", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�ӂ݃W�����vB"}, //mParent
        "HopperWaitB", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�ړ�A"}, //mParent
        "HopperRunA", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�ړ�B"}, //mParent
        "HopperRunB", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�n�C�W�����vA"}, //mParent
        "HopperJumpA", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�n�C�W�����vB"}, //mParent
        "HopperJumpB", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�ǃW�����v"}, //mParent
        "HopperWallJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�q�b�v�h���b�v�J�n"}, //mParent
        "HopperMarioHipDropStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�z�b�p�[�q�b�v�h���b�v"}, //mParent
        "HopperMarioHipDrop", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�W�����v�ӂ�1"}, //mParent
        "Jump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�W�����v�ӂ�2"}, //mParent
        "JumpPress2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�W�����v�ӂ�3"}, //mParent
        "JumpPress3rd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����߂�"}, //mParent
        "PullBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����߂����n"}, //mParent
        "PullBackLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�|�b�h���[�v�J�n"}, //mParent
        "WarpPodStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�|�b�h���[�v�I��"}, //mParent
        "WarpPodEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���Ƃ�"}, //mParent
        "JumpBroad", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���n"}, //mParent
        "Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���nB"}, //mParent
        "Jump2Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���nC"}, //mParent
        "JumpRollLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���n�^�[��"}, //mParent
        "JumpTurnLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���n���Ƃ�"}, //mParent
        "JumpBroadLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�[�h���n"}, //mParent
        "LandStiffen", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���ׂ蒅�n"}, //mParent
        "LandSlope", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�V���[�g���n"}, //mParent
        "Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�q�b�v�h���b�v�J�n"}, //mParent
        "HipDropStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�q�b�v�h���b�v"}, //mParent
        "HipDrop", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�q�b�v�h���b�v���n"}, //mParent
        "HipDropLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�q�b�v�h���b�v����"}, //mParent
        "LandRotation", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�W�����v������"}, //mParent
        "LandRotation", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�W�����v�t����"}, //mParent
        "Fall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���q�b�v�h���b�v�J�n"}, //mParent
        "HipDropHomingStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���q�b�v�h���b�v"}, //mParent
        "HipDropHoming", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���q�b�v�h���b�v���n"}, //mParent
        "HipDropHomingLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X���b�v�A�b�v"}, //mParent
        "SlipUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂�X���b�v�A�b�v����"}, //mParent
        "HangSlipUpStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂�X���b�v�A�b�v"}, //mParent
        "HangSlipUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`��s��"}, //mParent
        "BeeFly", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`��s��������"}, //mParent
        "BeeFlyWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�ǂ�����"}, //mParent
        "BeeLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�ǂ�������"}, //mParent
        "BeeWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�Ԉړ�"}, //mParent
        "BeeCreepWallWalk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�W�����v"}, //mParent
        "BeeJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�ǃW�����v"}, //mParent
        "BeeWallJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�����E�G�C�g"}, //mParent
        "BeeCreepWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�����O�i"}, //mParent
        "BeeCreepWalk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�������n"}, //mParent
        "BeeCreepLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�q�b�v�h���b�v�J�n"}, //mParent
        "BeeHipDropStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�q�b�v�h���b�v"}, //mParent
        "BeeHipDrop", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�q�b�v�h���b�v���n"}, //mParent
        "BeeHipDropLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�q�b�v�h���b�v�ǒ��n"}, //mParent
        "BeeHipDropLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�D�ᑬ���s"}, //mParent
        "WalkSoft", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�D�������s"}, //mParent
        "WalkBury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����"}, //mParent
        "Fall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�[�t�@�C�^�[��s�J�n"}, //mParent
        "FooStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�[�t�@�C�^�[��s"}, //mParent
        "FooFly", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�[�t�@�C�^�[��s�ĊJ"}, //mParent
        "FooFlyStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�[�t�@�C�^�[�Î~"}, //mParent
        "FooWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�[�t�@�C�^�[����"}, //mParent
        "FooEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�[�t�@�C�^�[���n"}, //mParent
        "LandSlope", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�[�t�@�C�^�[�X�s��"}, //mParent
        "FooSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǃW�����v"}, //mParent
        "WallJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǂ��ׂ�"}, //mParent
        "WallSlide", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǂ�����"}, //mParent
        "WallKeep", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǉ���"}, //mParent
        "Push", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ǎ�����"}, //mParent
        "WallWalkL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǉE����"}, //mParent
        "WallWalkR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǃE�G�C�g"}, //mParent
        "WallWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�O�ǃE�G�C�g"}, //mParent
        "Push", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǃq�b�g"}, //mParent
        "WallHit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǃq�b�g���n"}, //mParent
        "WallHitLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ǂ͂���"}, //mParent
        "WallHit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�R�ӂ�΂�"}, //mParent
        "Stagger", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�J���J�����E"}, //mParent
        "WaitHold", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�⍶�E�G�C�g"}, //mParent
        "WaitSlopeL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��E�E�G�C�g"}, //mParent
        "WaitSlopeR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��O�E�G�C�g"}, //mParent
        "WaitSlopeD", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���E�G�C�g"}, //mParent
        "WaitSlopeU", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����"}, //mParent
        "Run", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���s"}, //mParent
        "Walk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ݍs"}, //mParent
        "WalkSoft", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���^���_�b�V��"}, //mParent
        "RunDash", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂���s"}, //mParent
        "WalkBury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂�W�����vA"}, //mParent
        "WalkBuryJumpLow", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂�W�����vB"}, //mParent
        "WalkBuryJumpLow2", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂�E�o�W�����v"}, //mParent
        "WalkBuryJumpHi", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ԃ�"}, //mParent
        "Press", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ԃꕜ�A"}, //mParent
        "PressRecover", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ԃ����"}, //mParent
        "Jump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�u���[�L"}, //mParent
        "Brake", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�u���[�L���菰"}, //mParent
        "Run", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�^�[���u���[�L"}, //mParent
        "Turn", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�^�[���u���[�L���菰"}, //mParent
        "Run", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���s�����u���[�L"}, //mParent
        "RunEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�㊵�����s"}, //mParent
        "Brake", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X��͍s��"}, //mParent
        "SkateL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X��͍s�E"}, //mParent
        "SkateR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X��ؑ֍�"}, //mParent
        "SkateSwitchL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X��ؑ։E"}, //mParent
        "SkateSwitchR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X���s��"}, //mParent
        "SkateBackL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X���s�E"}, //mParent
        "SkateBackR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X��^�[��"}, //mParent
        "SkateTurn", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���Ⴊ��"}, //mParent
        "SquatWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���Ⴊ�݊J�n"}, //mParent
        "SquatStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���Ⴊ�ݏI��"}, //mParent
        "SquatEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�₷�ׂ��������Ԃ�"}, //mParent
        "SlideStmach", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�₷�ׂ������I��"}, //mParent
        "SlideStmachEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�₷�ׂ艺�����I��"}, //mParent
        "SlideHipEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����E�G�C�g1A"}, //mParent
        "Sleep", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����E�G�C�g1B"}, //mParent
        "SleepLie", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�퓬�E�G�C�g"}, //mParent
        "BattleWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��]"}, //mParent
        "Run", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ђ낢"}, //mParent
        "CarryStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ђ낢��"}, //mParent
        "CarryAirStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ђ낢�N�C�b�N"}, //mParent
        "CarryStartShort", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ђ낢�E�G�C�g"}, //mParent
        "CarryWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�J�u����"}, //mParent
        "PullOut", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�J�u�E�G�C�g"}, //mParent
        "PullOutWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n���}�[������]��"}, //mParent
        "Swing", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n���}�[���������[�X"}, //mParent
        "SwingThrow", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�R�C���Q�b�g"}, //mParent
        "CoinGet", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�p���`"}, //mParent
        "Spin2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��p���`"}, //mParent
        "Spin2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���p���`"}, //mParent
        "Spin2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����p���`"}, //mParent
        "Spin2nd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����j���O�L�b�N"}, //mParent
        "Kick", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�A�b�p�[�p���`"}, //mParent
        "Jump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���Ⴊ�݃A�b�p�["}, //mParent
        "SquatEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�T�}�[�\���g"}, //mParent
        "SpinLow", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����"}, //mParent
        "Throw", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�@�C�A����"}, //mParent
        "Throw", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���蓊��"}, //mParent
        "ThrowBoth", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�@�C�A�X�s��"}, //mParent
        "FireSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�@�C�A�X�s����"}, //mParent
        "FireSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�R���܂�J�n"}, //mParent
        "HangStartUnder", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�R���܂蒆"}, //mParent
        "HangWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�R���܂�I��"}, //mParent
        "HangUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�R���܂�I����"}, //mParent
        "HangUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�R���܂�~��"}, //mParent
        "HangStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���Ⴊ�݃W�����v"}, //mParent
        "JumpBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���Ⴊ�݃W�����v���n"}, //mParent
        "JumpBackLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���Ⴊ�ݕ���"}, //mParent
        "SquatWalk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����΂葖��"}, //mParent
        "RunSlope", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���΂��W�����v"}, //mParent
        "SlideStomachRecover", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�K����W�����v"}, //mParent
        "SlideHipRecover", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"������������"}, //mParent
        "RunSlope", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���������ӂ�΂�"}, //mParent
        "Stagger", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���̏ꑫ���ݏ㔼�g"}, //mParent
        "Walk", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�E�G�C�g"}, //mParent
        "SwimWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j��~��"}, //mParent
        "SwimBreast", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����~��"}, //mParent
        "SwimBreastSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j����"}, //mParent
        "SwimDive", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�W�����v�_�C�u"}, //mParent
        "LandWater", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�W�����v�_�C�u��]"}, //mParent
        "LandWaterDive", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�㏸�ċz"}, //mParent
        "SwimRise", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j���ʏ����ړ�"}, //mParent
        "SwimStartSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����X�s��"}, //mParent
        "SwimSpinSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�X�s��"}, //mParent
        "SwimSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�X�s���ړ�"}, //mParent
        "SwimSpinAttack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����X�s���ړ�"}, //mParent
        "SwimSpinAttackSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�X�s���W�����v"}, //mParent
        "SwimJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�_���[�W"}, //mParent
        "SwimDamageSmall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�_���[�W�T"}, //mParent
        "SwimFlutterBoardDamageSmall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�_���[�W��"}, //mParent
        "SwimDamageMiddle", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����_���[�W��"}, //mParent
        "SwimDamageMiddleSurface", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����_���[�W����"}, //mParent
        "SwimDamageMiddleSurfaceLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�W�F�b�g"}, //mParent
        "SwimFlutterboard", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�W�F�b�g�I��"}, //mParent
        "SwimDamageMiddle", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�T����"}, //mParent
        "SwimFlutterboardThrow", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�W�F�b�g�J�n"}, //mParent
        "SwimFlutterboardStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�W�F�b�g�ǃ^�[��"}, //mParent
        "SwimFlutterboardTurn", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�_�E��"}, //mParent
        "SwimDie", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�^�[����"}, //mParent
        "SwimTurnL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�^�[���E"}, //mParent
        "SwimTurnR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�ǃq�b�g"}, //mParent
        "SwimWallHit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j�^�[����"}, //mParent
        "SwimTurnForward", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���j����������"}, //mParent
        "SlideStmachEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��э��ݏ���"}, //mParent
        "DiveWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��э��݃W�����v"}, //mParent
        "Dive", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����э��݃W�����v"}, //mParent
        "DiveBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��э��ݎ��s���n"}, //mParent
        "Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��э��ݎ��s��]���n"}, //mParent
        "LandRotation", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����O�_�b�V��"}, //mParent
        "SwimDashRing", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����O�_�b�V������"}, //mParent
        "SwimDashRingStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�J�����������O����"}, //mParent
        "SwimFlutterBoardDashRingStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�J�����������O"}, //mParent
        "SwimFlutterBoardDashRing", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����O�i"}, //mParent
        "SwimGetUp", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�O�����_���[�W"}, //mParent
        "DamageSmallFront", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"������_���[�W"}, //mParent
        "DamageSmallBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����W�����v"}, //mParent
        "Rise", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���_���[�W"}, //mParent
        "DamageMiddleFront", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���_���[�W��"}, //mParent
        "DamageMiddleFrontAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���_���[�W���n"}, //mParent
        "DamageMiddleFrontLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����_���[�W"}, //mParent
        "DamageMiddleBack", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����_���[�W��"}, //mParent
        "DamageMiddleBackAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����_���[�W���n"}, //mParent
        "DamageMiddleBackLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���_���[�W"}, //mParent
        "DamageFire", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�m�[�_���[�W"}, //mParent
        "DamageBit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���̃����i�["}, //mParent
        "FireRun", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�@�C�A�����O��"}, //mParent
        "FireRunStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�d�C�_���[�W"}, //mParent
        "DamageElectric", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�d�C�_���[�W�I��"}, //mParent
        "DamageElectricEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�t�@�C�A�_���X"}, //mParent
        "FireRun", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�_���[�W"}, //mParent
        "DamageStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�_���[�W���n"}, //mParent
        "Land", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�_���[�W�E�G�C�g"}, //mParent
        "DamageWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�m�[�}���E�G�C�g"}, //mParent
        "Wait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X��"}, //mParent
        "Freeze", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X������"}, //mParent
        "IceFlick", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���т�"}, //mParent
        "DamageNumb", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���т��"}, //mParent
        "DamageNumbEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�͂˂Ƃ΂���"}, //mParent
        "DamageFlick", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�͂˂Ƃ΂���I��"}, //mParent
        "DamageFlickEnd", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��]�ӂ��Ƃ�"}, //mParent
        "DamageWeakFront", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��]�ӂ��Ƃы�"}, //mParent
        "DamageWeakFrontAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"��]�ӂ��Ƃђ��n"}, //mParent
        "DamageWeakFrontLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����_�E��"}, //mParent
        "DieSit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���d�_�E��"}, //mParent
        "DieElectric", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���_�E��"}, //mParent
        "DieFire", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�����_�E��"}, //mParent
        "DieOver", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�낹�_�E��"}, //mParent
        "DieUnder", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�ޗ��_�E��"}, //mParent
        "DieFall", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�u���b�N�z�[������"}, //mParent
        "DieBlackHole", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Ԃ�_�E��"}, //mParent
        "DieSit", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�Q�[���I�[�o�["}, //mParent
        "DieOver", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���[�X����"}, //mParent
        "DieEvent", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�������[�X����"}, //mParent
        "DieSwimEvent", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���܂�_�E��"}, //mParent
        "DieBury", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�e�j�X�V���b�g��"}, //mParent
        "TennisShotL", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�e�j�X�V���b�g�E"}, //mParent
        "TennisShotR", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�e�j�X�V���b�g��"}, //mParent
        "TennisShotM", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�e�j�X�V���b�g��"}, //mParent
        "TennisShotAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�e�j�X�E�G�C�g"}, //mParent
        "TennisWait", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�G�������g�Q�b�g"}, //mParent
        "ElementGet", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�G�������g�Q�b�g�ڒn��"}, //mParent
        "ElementGetGround", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�󒆂Ђ˂�"}, //mParent
        "Spin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n��Ђ˂�"}, //mParent
        "SpinGround", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�A�C�X�Ђ˂�"}, //mParent
        "IceSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�A�C�X�Ђ˂�ړ�"}, //mParent
        "IceSkateSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�A�C�X�Ђ˂�Î~"}, //mParent
        "IceSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�X�s����"}, //mParent
        "BeeSpin", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�n�`�X�s��"}, //mParent
        "BeeSpinGround", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�A�C�X�Ђ˂��"}, //mParent
        "IceSpinAir", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�P�[�g�A�N�Z���W�����v"}, //mParent
        "IceJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�P�[�g�W�����v2"}, //mParent
        "IceJump2", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�P�[�g�W�����v3"}, //mParent
        "IceJump3", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�P�[�g���n"}, //mParent
        "IceJumpLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�P�[�g�Î~���n"}, //mParent
        "IceJumpStopLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�T�[�t�B���W�����v"}, //mParent
        "SurfJump", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�T�[�t�B���n�C�W�����v"}, //mParent
        "SurfJumpHigh", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�T�[�t�B�����n"}, //mParent
        "SurfLand", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"����"}, //mParent
        "Watch", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���[�X����"}, //mParent
        "Watch", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�e�[�W�C��A"}, //mParent
        "StageStartGround", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�e�[�W�C��B"}, //mParent
        "LandScenarioStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�E�H�[�N�C��"}, //mParent
        "GoThrough", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[�f��1]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[��b1]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[�f��2]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[��b2]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[�f��3]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[��b3]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[�f��4]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[�f��5]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[��b4]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[�f��6]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[�f��7]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�X�s���Q�b�g[�f��8]"}, //mParent
        "DemoGetPower", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���[�X�J�n"}, //mParent
        "RaceStart", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"���[�X�N���E�`���O�J�n"}, //mParent
        "RaceStartCrouch", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�S�[�X�g���[�X�J�n"}, //mParent
        "RaceStartGhost", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�S�[�X�g����"}, //mParent
        "WinGhost", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {"�S�[�X�g�o��"}, //mParent
        "AppearGhost", //fileName
        0, // animationHash
        0, // fileHash
    },
    {
        {""}, //mParent
        "", //fileName
        0, // animationHash
        0, // fileHash
    },
};

XanimeGroupInfo marioAnimeTable[0x134] = {
    {
        {"��{"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ⴊ�݊�{"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j��{"}, // mParent
        1.00000000000f, // 0x4
        0x1e, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�e�j�X��{"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�⍶�E�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��O��E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�⍶�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��E�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��O�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���s"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ݍs"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���^���_�b�V��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�_�b�V���W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��т�����W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�V���[�g�W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂���s"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂�W�����vA"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂�W�����vB"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂�E�o�W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ⴊ��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�u���[�L"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�u���[�L���菰"}, // mParent
        4.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�^�[���u���[�L"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�^�[���u���[�L���菰"}, // mParent
        3.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���s�����u���[�L"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�㊵�����s"}, // mParent
        1.00000000000f, // 0x4
        0x10, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X��͍s��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X��͍s�E"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X��ؑ֍�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X��ؑ։E"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X���s��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X���s�E"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X��^�[��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����E�G�C�g1A"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����E�G�C�g1B"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�퓬�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x1e, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����O�i"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�W�����v"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�W�����vB"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�W�����vC"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�^�[���W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t���[�W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�W�����v�ӂ�1"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�W�����v�ӂ�2"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�W�����v�ӂ�3"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����߂�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����߂����n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�|�b�h���[�v�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�|�b�h���[�v�I��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�J�C���u�W�����v"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�󒆈��]"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�P�L��"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�P�L���E�o"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂�"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂�E�o"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�W�����vA"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�W�����vB"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�ӂ݃W�����vA"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�ӂ݃W�����vB"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�ړ�A"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�ړ�B"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�n�C�W�����vA"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�n�C�W�����vB"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�ǃW�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�q�b�v�h���b�v�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�z�b�p�[�q�b�v�h���b�v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ƃ�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�q�b�v�h���b�v�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�q�b�v�h���b�v"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�q�b�v�h���b�v���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�q�b�v�h���b�v����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�W�����v������"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�W�����v�t����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X���b�v�A�b�v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂�X���b�v�A�b�v����"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂�X���b�v�A�b�v"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���q�b�v�h���b�v�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���q�b�v�h���b�v"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���q�b�v�h���b�v���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����"}, // mParent
        1.00000000000f, // 0x4
        0xf, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���n"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���nB"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�[�h���n"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���ׂ蒅�n"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�V���[�g���n"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`��s��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`��s��������"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�ǂ�����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�ǂ�������"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�ǃW�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�����E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�����O�i"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�������n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�Ԉړ�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�q�b�v�h���b�v�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�q�b�v�h���b�v"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�q�b�v�h���b�v���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�q�b�v�h���b�v�ǒ��n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�[�t�@�C�^�[��s�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�[�t�@�C�^�[��s"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�[�t�@�C�^�[��s�ĊJ"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�[�t�@�C�^�[�Î~"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�[�t�@�C�^�[����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�[�t�@�C�^�[���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�[�t�@�C�^�[�X�s��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�D�ᑬ���s"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�D�������s"}, // mParent
        2.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǂ�����"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǂ��ׂ�"}, // mParent
        1.00000000000f, // 0x4
        0xa, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǃW�����v"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǉ���"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ǎ�����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǉE����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǃE�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�O�ǃE�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǃq�b�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǃq�b�g���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ǂ͂���"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�_���[�W���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�_���[�W�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�m�[�}���E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X������"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���т�"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���т��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�R�C���Q�b�g"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�p���`"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��p���`"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���p���`"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����p���`"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����j���O�L�b�N"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�A�b�p�[�p���`"}, // mParent
        1.00000000000f, // 0x4
        0x3, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ⴊ�݃A�b�p�["}, // mParent
        1.00000000000f, // 0x4
        0x3, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�}�[�\���g"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Q�[���I�[�o�["}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ԃ�"}, // mParent
        0.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ԃꕜ�A"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ԃ����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��]"}, // mParent
        4.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�₷�ׂ��������Ԃ�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�₷�ׂ艺���������ނ�"}, // mParent
        2.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"������������"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���������ӂ�΂�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�[�t�B��"}, // mParent
        1.00000000000f, // 0x4
        0xf, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�[�t�B���i�����j"}, // mParent
        1.00000000000f, // 0x4
        0xf, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�[�t�B���X���J�n"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�[�t�B���X���J�n�i�����j"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�[�t�B���W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�[�t�B���n�C�W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�[�t�B������"}, // mParent
        1.00000000000f, // 0x4
        0xf, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�T�[�t�B�����n"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�^�}�R���ړ�"}, // mParent
        1.00000000000f, // 0x4
        0xf, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�^�}�R�����Ⴊ��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X���C�_�[�K"}, // mParent
        2.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���̏ꑫ����"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���̏ꑫ���ݏ㔼�g"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j��~��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����~��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�W�����v�_�C�u"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�W�����v�_�C�u��]"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�㏸�ċz"}, // mParent
        1.00000000000f, // 0x4
        0x10, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j���ʏ����ړ�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����X�s��"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�X�s��"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�X�s���ړ�"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����X�s���ړ�"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�_���[�W�T"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�_���[�W��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����_���[�W��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����_���[�W����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�^�[����"}, // mParent
        1.00000000000f, // 0x4
        0x14, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�^�[���E"}, // mParent
        1.00000000000f, // 0x4
        0x14, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�^�[����"}, // mParent
        1.00000000000f, // 0x4
        0x14, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�W�F�b�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�W�F�b�g�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�W�F�b�g�I��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�T����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�ǃq�b�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�W�F�b�g�ǃ^�[��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j����������"}, // mParent
        1.00000000000f, // 0x4
        0x10, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��э��ݏ���"}, // mParent
        1.00000000000f, // 0x4
        0x14, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��э��݃W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����э��݃W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��э��ݎ��s���n"}, // mParent
        1.00000000000f, // 0x4
        0xf, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��э��ݎ��s��]���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���j�X�s���W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����O�_�b�V��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����O�_�b�V������"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�J�����������O����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�J�����������O"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���nC"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���n�^�[��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���n���Ƃ�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ⴊ�݊J�n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ⴊ�ݏI��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�₷�ׂ������I��"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�₷�ׂ艺�����I��"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ђ낢"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ђ낢��"}, // mParent
        1.00000000000f, // 0x4
        0x3, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ђ낢�N�C�b�N"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ђ낢�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�J�u����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�J�u�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n���}�[������]��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n���}�[���������[�X"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�@�C�A����"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���蓊��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�@�C�A�X�s��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�@�C�A�X�s����"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�R���܂�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x5, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�R���܂蒆"}, // mParent
        1.00000000000f, // 0x4
        0x5, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�R���܂�I��"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�R���܂�I����"}, // mParent
        1.00000000000f, // 0x4
        0x4, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�R���܂�~��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�R�ӂ�΂�"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ⴊ�݃W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ⴊ�݃W�����v���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���Ⴊ�ݕ���"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����΂葖��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���΂��W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�K����W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�O�����_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"������_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�w���R�v�^�[�W�����v"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���_���[�W��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���_���[�W���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����_���[�W��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����_���[�W���n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�m�[�_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���̃����i�["}, // mParent
        8.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�@�C�A�����O��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�d�C�_���[�W"}, // mParent
        1.00000000000f, // 0x4
        0x1, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�d�C�_���[�W�I��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�t�@�C�A�_���X"}, // mParent
        6.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��]�ӂ��Ƃ�"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��]�ӂ��Ƃы�"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"��]�ӂ��Ƃђ��n"}, // mParent
        1.00000000000f, // 0x4
        0x2, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���d�_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�����_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�낹�_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�ޗ��_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x14, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�u���b�N�z�[������"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���[�X����"}, // mParent
        1.00000000000f, // 0x4
        0xf, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�������[�X����"}, // mParent
        1.00000000000f, // 0x4
        0xf, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���܂�_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x1e, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�Ԃ�_�E��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�͂˂Ƃ΂���"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�͂˂Ƃ΂���I��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�e�j�X�V���b�g��"}, // mParent
        1.25000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�e�j�X�V���b�g�E"}, // mParent
        1.25000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�e�j�X�V���b�g��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�e�j�X�V���b�g��"}, // mParent
        1.25000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�e�j�X�E�G�C�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�G�������g�Q�b�g"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�G�������g�Q�b�g�ڒn��"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�󒆂Ђ˂�"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n��Ђ˂�"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�X�s��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�n�`�X�s����"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�A�C�X�Ђ˂�"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�A�C�X�Ђ˂�ړ�"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�A�C�X�Ђ˂�Î~"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�A�C�X�Ђ˂��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�P�[�g�A�N�Z���W�����v"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�P�[�g�W�����v2"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�P�[�g�W�����v3"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�P�[�g���n"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�P�[�g�Î~���n"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���[�X����"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�J���J�����E"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�e�[�W�C��A"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�e�[�W�C��B"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�E�H�[�N�C��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[�f��1]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[��b1]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[�f��2]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[��b2]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[�f��3]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[��b3]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[�f��4]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[�f��5]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[��b4]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[�f��6]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[�f��7]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�X�s���Q�b�g[�f��8]"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���[�X�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"���[�X�N���E�`���O�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�S�[�X�g���[�X�J�n"}, // mParent
        1.00000000000f, // 0x4
        0x6, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�S�[�X�g����"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {"�S�[�X�g�o��"}, // mParent
        1.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
    {
        {""}, // mParent
        0.00000000000f, // 0x4
        0, // 0x8
        0.00000000000f, // 0xC
        0.00000000000f, // 0x10
        0.00000000000f, // 0x14
        0, // 0x18
        0, // 0x1C
        0, // 0x1D
        {
            0,
            0,
            0,
            0,
        }, // 0x20
        {
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
            0.00000000000f,
        }, // 0x30
        0, // 0x40
        0, // 0x44
        nullptr, // 0x48
    },
};

XanimeAuxInfo marioAnimeAuxTable[1] = {{""}};

XanimeOfsInfo marioAnimeOfsTable[35] = {
    {
        {"�_���[�W"}, //mParent
        0.00000000000f, // 0x4
        10.0000000000f, // 0x8
        10.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�₷�ׂ艺���������ނ�"}, //mParent
        0.00000000000f, // 0x4
        2.00000000000f, // 0x8
        2.00000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�₷�ׂ��������Ԃ�"}, //mParent
        0.00000000000f, // 0x4
        2.00000000000f, // 0x8
        2.00000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�^�[���u���[�L"}, //mParent
        0.00000000000f, // 0x4
        20.0000000000f, // 0x8
        20.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�W�����v"}, //mParent
        0.00000000000f, // 0x4
        24.0000000000f, // 0x8
        24.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�ǃq�b�g"}, //mParent
        0.00000000000f, // 0x4
        25.0000000000f, // 0x8
        25.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�ǃq�b�g���n"}, //mParent
        0.00000000000f, // 0x4
        67.0000000000f, // 0x8
        0.00000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�J�u����"}, //mParent
        0.00000000000f, // 0x4
        49.0000000000f, // 0x8
        49.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"����"}, //mParent
        0.00000000000f, // 0x4
        29.0000000000f, // 0x8
        29.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�q�b�v�h���b�v�J�n"}, //mParent
        0.00000000000f, // 0x4
        13.0000000000f, // 0x8
        13.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�q�b�v�h���b�v"}, //mParent
        0.00000000000f, // 0x4
        9.00000000000f, // 0x8
        9.00000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�X�s���q�b�v�h���b�v"}, //mParent
        0.00000000000f, // 0x4
        19.0000000000f, // 0x8
        19.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�n�`�q�b�v�h���b�v�ǒ��n"}, //mParent
        0.00000000000f, // 0x4
        20.0000000000f, // 0x8
        20.0000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�z�b�p�[�ӂ݃W�����vA"}, //mParent
        10.0000000000f, // 0x4
        50.0000000000f, // 0x8
        0.00000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�z�b�p�[�ӂ݃W�����vB"}, //mParent
        10.0000000000f, // 0x4
        50.0000000000f, // 0x8
        0.00000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�e�j�X�V���b�g��"}, //mParent
        4.00000000000f, // 0x4
        26.0000000000f, // 0x8
        26.0000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�e�j�X�V���b�g�E"}, //mParent
        4.00000000000f, // 0x4
        26.0000000000f, // 0x8
        26.0000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�e�j�X�V���b�g��"}, //mParent
        3.00000000000f, // 0x4
        38.0000000000f, // 0x8
        38.0000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�e�j�X�V���b�g��"}, //mParent
        5.00000000000f, // 0x4
        26.0000000000f, // 0x8
        26.0000000000f, // 0xC
        0, // 0x10
    },
    {
        {"���̏ꑫ����"}, //mParent
        0.00000000000f, // 0x4
        17.0000000000f, // 0x8
        8.00000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�u���[�L"}, //mParent
        0.00000000000f, // 0x4
        2.00000000000f, // 0x8
        0.00000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�V���[�g���n"}, //mParent
        0.00000000000f, // 0x4
        5.00000000000f, // 0x8
        0.00000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�A�b�p�[�p���`"}, //mParent
        22.0000000000f, // 0x4
        23.0000000000f, // 0x8
        0.00000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"���Ⴊ�݃A�b�p�["}, //mParent
        0.00000000000f, // 0x4
        6.00000000000f, // 0x8
        0.00000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�X�P�L��"}, //mParent
        0.00000000000f, // 0x4
        59.0000000000f, // 0x8
        30.0000000000f, // 0xC
        0x2, // 0x10
    },
    {
        {"�X�P�L���E�o"}, //mParent
        60.0000000000f, // 0x4
        120.000000000f, // 0x8
        0.00000000000f, // 0xC
        0, // 0x10
    },
    {
        {"����E�G�C�g1A"}, //mParent
        0.00000000000f, // 0x4
        494.000000000f, // 0x8
        375.000000000f, // 0xC
        0x2, // 0x10
    },
    {
        {"����E�G�C�g1B"}, //mParent
        0.00000000000f, // 0x4
        254.000000000f, // 0x8
        135.000000000f, // 0xC
        0x2, // 0x10
    },
    {
        {"�A�C�X�Ђ˂�"}, //mParent
        0.00000000000f, // 0x4
        35.0000000000f, // 0x8
        35.0000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�t�@�C�A�X�s����"}, //mParent
        0.00000000000f, // 0x4
        30.0000000000f, // 0x8
        30.0000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�R���܂�I����"}, //mParent
        0.00000000000f, // 0x4
        27.0000000000f, // 0x8
        27.0000000000f, // 0xC
        0, // 0x10
    },
    {
        {"�O�ǃE�G�C�g"}, //mParent
        57.0000000000f, // 0x4
        59.0000000000f, // 0x8
        57.0000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {"�t�[�t�@�C�^�[�Î~"}, //mParent
        0.00000000000f, // 0x4
        159.000000000f, // 0x8
        40.0000000000f, // 0xC
        0x2, // 0x10
    },
    {
        {"�Ђ낢�N�C�b�N"}, //mParent
        0.00000000000f, // 0x4
        26.0000000000f, // 0x8
        0.00000000000f, // 0xC
        0x1, // 0x10
    },
    {
        {""}, //mParent
        0.00000000000f, // 0x4
        0.00000000000f, // 0x8
        0.00000000000f, // 0xC
        0, // 0x10
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
