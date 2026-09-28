#ifndef TYPES_H
#define TYPES_H
// SHARED (whole team). Plain data. Never branch on these to pick behaviour.
enum class IncidentType { Fire, Medical, Intrusion, Hazmat };
enum class Severity     { Low, Moderate, Critical };
enum class AccessMode   { Unlocked, Restricted, Locked };

inline const char* toString(IncidentType t) {
    static const char* n[] = { "Fire", "Medical", "Intrusion", "Hazmat" };
    return n[static_cast<int>(t)];
}
inline const char* toString(Severity s) {
    static const char* n[] = { "Low", "Moderate", "Critical" };
    return n[static_cast<int>(s)];
}
inline const char* toString(AccessMode m) {
    static const char* n[] = { "Unlocked", "Restricted", "Locked" };
    return n[static_cast<int>(m)];
}
#endif
