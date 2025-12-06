#pragma once

class ResTIMG;

namespace MR {
    void startGalaxyMapLayout();
    void startAstroMapLayout();
    void startAstroMapLayoutForNewGalaxyDiscover();
    void startAstroMapLayoutForNewTicoGalaxyDiscover();
    void startAstroMapLayoutForNewDomeDiscover();
    void startGalaxyMapLayoutForTicoComet();
    void startAstroMapLayoutForChallengeGalaxyDiscover();
    bool isActiveGalaxyMapLayout();
    const ResTIMG* getGalaxyMapResTIMG();
};  // namespace MR
