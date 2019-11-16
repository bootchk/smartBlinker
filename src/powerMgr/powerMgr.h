
/*
 * Understands the system and app power levels.
 *
 */
class PowerMgr {
public:
    // Is there enough power for the main capability of the application?
    static bool isPowerForBlinking();

    static bool isNearBrownOut();
};
