#pragma once

#include "../configurations.hpp"

#include <iosfwd>
#include <random>

class GeneratorParameters {
public:
   using Point = std::tuple <double, double>;

   GeneratorParameters(Configurations &conf): conf(conf) {
      // Empty
   }

   virtual ~GeneratorParameters() {
      // Empty
   }

   std::mt19937_64 &prng() {
      return rng;
   }

   int numDays() const { return conf.ndays; }
   int numCaregivers() const { return conf.ncaregivers; }

   int numDailyPatients() const {
      return conf.ndaily ;
   }

   int numSkills() const { return conf.nskills; }

   bool poolSizeOverride() const { return conf.patients_override; }

   double simultaneousDoubleServicesPerc() const { return conf.ds_simult_perc; }
   double precedenceDoubleServicesPerc() const { return conf.ds_prec_perc; }

   int maxSkillsPerCaregiver() const { return conf.max_carer_skills; }

   int minPatientsPerSkillUnit() const { return std::get<0>(conf.patients_per_skill_unit); }
   int maxPatientsPerSkillUnit() const { return std::get<1>(conf.patients_per_skill_unit); }

   int dayStartingMinutes() const { return std::get<0>(conf.day_horizon); }
   int dayEndingMinutes() const { return std::get<1>(conf.day_horizon); }

   int twDurationMinutes() const { return conf.time_window; }

   int minProcTime() const { return std::get<0>(conf.service_times_hhcp); }
   int maxProcTime() const { return std::get<1>(conf.service_times_hhcp); }

   int minSeparation() const { return std::get<0>(conf.delta_range); }
   int maxSeparation() const { return std::get<1>(conf.delta_range); }
   int minDynamicTwDurationMinutes() const { return conf.min_ds_dyntw_duration; }

private:
   Configurations &conf;
};
