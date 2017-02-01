#include <cmath>
#include <string>
#include <vector>

using std::string;
using std::vector;

struct Time {
  static constexpr double kMinutesPerHour = 60;
  static constexpr double kHoursPerDay = 24;
  static constexpr double kDaysPerMonth = 30;
  static constexpr double kMonthsPerYear = 4;

  static constexpr char* kMonthNames[4] = { "Spring", "Summer", "Autumn", "Winter" };

  double minutes;
  double hours() { return minutes / kMinutesPerHour; }
  double days() { return hours() / kHoursPerDay; }
  double months() { return days() / kDaysPerMonth; }
  double years() { return days() / kMonthsPerYear; }

  double minute_in_hour() { return minutes - floor(hours()) * kMinutesPerHour; }
  double hour_in_day() { return hours() - floor(days()) * kHoursPerDay; }
  double day_in_month() { return days() - floor(months()) * kDaysPerMonth; }
  double month_in_year() { return months() - floor(years()) * kMonthsPerYear; }

  const char* month_name() { return kMonthNames[static_cast<int>(month_in_year())]; }
};

constexpr double kWorldRadius = 65e5;

struct Position {
  double altitude;  // meters
  double longitude;
};

enum Property { kConsciousness, kHearing, kSight, kCharisma, kManipulation, kMoving, kPropertyCount };
enum Skills { kShooting, kMelee, kSculpting, kElectronics, kSurvival, kSkillCount };

struct BodyPartType {
  string name;
  double hit_points;
  double size;
  double property_importance[kPropertyCount];
};

struct BodyType {
  string name;
  double width;  // capsule size in meters
  double height;
  double property_base[kPropertyCount];
  vector<const BodyPartType*> parts;
};

namespace humanoid {
const BodyPartType kLeftEye = {
    .name = "Left eye",
    .hit_points = 5,
    .size = 0.03,
    .property_importance = { 0, 0, 1, 0.3, 0.5, 0.5 }
};

const BodyType kBodyType = {
    .name = "humanoid",
    .width = 1.0,
    .height = 1.75,
    .property_base = { 1, 1 },
    .parts = { &kLeftEye }
};
}

enum DamageType { kBurnDamage, kBluntDamage, kPiercingDamage };

struct Wound {
  Body* attacker;
  Item* weapon;
  double hit_points;
};

struct BodyPart {
  double efficiency_base;
  vector<Wound> wounds;
};

struct Body {
  const BodyType* type;
  vector<BodyPart> parts;
  Position position;
  vector<string> names;
  string nickname;
  Time birth;
  double property(Property p) {
    double value = type->property_base[p];
    for (int i = 0; i < type->parts.size(); ++i) {
      const BodyPartType* part_type = type->parts[i];
      auto& part = parts[i];
      double hit_points_max = part_type->hit_points;
      double hit_points_left = hit_points_max;
      for (auto& wound : part.wounds) {
        hit_points_left -= wound.hit_points;
      }
      double part_efficiency = part.efficiency_base * hit_points_left / hit_points_max;
      value *= pow(part_efficiency, part.type->property_importance[p]);
    }
    return value;
  }
};
