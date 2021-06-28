#include "../../include/hhcrsp/WeeklyData.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

using namespace std;

WeeklyData::WeeklyData(GeneratorParameters &params) : m_params(params) {
   // Empty
}

WeeklyData::~WeeklyData() {
   // Empty
}

int WeeklyData::generateInitialData() {
   generateSkillPartitions();

   m_carerQualif.resize(m_params.numCaregivers());
   for (auto &i : m_carerQualif) {
      i.resize(m_params.numSkills(), 0);
   }
   m_carerPerSkill.resize(m_params.numSkills(), 0);

   generateCaregivers();
   generatePatientsPool();

   return patientsPoolSize();
}

int WeeklyData::patientsPoolSize() const {
   return m_poolSize;
}

void WeeklyData::setLocations(const std::vector <GeoLocation>& locs) {
   if (long(locs.size()) != m_poolSize+1) {
      cout << "ERROR: Locations vector has less points than locations of the instance.\n";
      cout << "ERROR: Requires " << m_poolSize+1 << " but only has " << locs.size() << "\n";
      exit(EXIT_FAILURE);
   }
   m_locations = locs;

   m_travelTimes.resize(m_locations.size());
   for (auto &row: m_travelTimes)
      row.resize(m_locations.size());
}

void WeeklyData::setDistance(int origin, int dest, double dist) {
   m_travelTimes[origin][dest] = dist;
}

void WeeklyData::generateDailyData() {
   if (m_poolSize <= 0) {
      cout << "ERROR: You need to generate initial data before generating daily data.\n";
      exit(EXIT_FAILURE);
   }
   for (int day = 0; day < m_params.numDays(); ++day) {
      generateDailyData(day);
   }
}

FileCode WeeklyData::writeDailyData(int day) const {
   // Prepare the file handler.
   char fidName[128];
   snprintf(fidName, sizeof fidName, "daily_%d.txt", day);
   ofstream fid(fidName);
   if (!fid) return FileCode::WriteError;

   // Write the headers
   fid << "nbNodes\n" << m_params.numDailyPatients()+2 << "\n";
   fid << "nbVehi\n" << m_params.numCaregivers() << "\n";
   fid << "nbServi\n" << m_params.numSkills() << "\n";

   // Node requirements.
   fid << "r\n";
   for (int s = 0; s < m_params.numSkills(); ++s)
      fid << "1 ";
   fid << "\n";

   for (int i: m_patients[day]) {
      for (int s = 0; s < m_params.numSkills(); ++s) {
         fid << m_demands[i][day][s] << " ";
      }
      fid << "\n";
   }

   for (int s = 0; s < m_params.numSkills(); ++s)
      fid << "1 ";
   fid << "\n";

   // DS list
   fid << "DS\n";
   for (size_t pos = 0; pos < m_patients[day].size(); ++pos) {
      int i = m_patients[day][pos];
      if (accumulate(m_demands[i][day].begin(), m_demands[i][day].end(), 0) == 2)
      fid << pos+2 << " ";
   }
   fid << "\n";

   // Caregivers skills.
   fid << "a\n";
   for (int v = 0; v < m_params.numCaregivers(); ++v) {
      for (int s = 0; s < m_params.numSkills(); ++s)
         fid << caregiverHasSkill(v, s) << " ";
      fid << "\n";
   }

   // Location positions.
   fid << "x\n";
   fid << m_locations.front().lat << " ";
   for (int i: m_patients[day])
      fid << m_locations[i].lat << " ";
   fid << m_locations.front().lat << "\n";

   fid << "y\n";
   fid << m_locations.front().lon << " ";
   for (int i: m_patients[day])
      fid << m_locations[i].lon << " ";
   fid << m_locations.front().lon << "\n";

   // Write the travel times matrix.
   fid << "d\n";
   fid.precision(5);
   fid << fixed;
   fid << m_travelTimes[0][0] << " ";
   for (int i: m_patients[day]) {
      fid << m_travelTimes[0][i] << " ";
   }
   fid << m_travelTimes[0][0] << "\n";

   for (int i : m_patients[day]) {
      fid << m_travelTimes[i][0] << " ";
      for (int j : m_patients[day]) {
         fid << m_travelTimes[i][j] << " ";
      }
      fid << m_travelTimes[i][0] << "\n";
   }

   fid << m_travelTimes[0][0] << " ";
   for (int i : m_patients[day]) {
      fid << m_travelTimes[0][i] << " ";
   }
   fid << m_travelTimes[0][0] << "\n";

   // Write processing times.
   fid << "p\n";
   fid.precision(1);
   for (int v = 0; v < m_params.numCaregivers(); ++v) {
      for (int s = 0; s < m_params.numSkills(); ++s)
         fid << 0.0 << " ";
      fid << "\n";
   }

   for (int i: m_patients[day]) {
      for (int v = 0; v < m_params.numCaregivers(); ++v) {
         for (int s = 0; s < m_params.numSkills(); ++s) {
            fid << m_procTimes[i][day][s] << " ";
         }
         fid << "\n";
      }
   }

   for (int v = 0; v < m_params.numCaregivers(); ++v) {
      for (int s = 0; s < m_params.numSkills(); ++s)
         fid << 0.0 << " ";
      fid << "\n";
   }

   // Minimum and maximum separation times.
   fid << "mind\n0 ";
   for (int i: m_patients[day]) {
      fid << get<0>(m_separationTimes[i][day]) << " ";
   }
   fid << "0\n";

   fid << "maxd\n0 ";
   for (int i: m_patients[day]) {
      fid << get<1>(m_separationTimes[i][day]) << " ";
   }
   fid << "0\n";

   // Patient time windows.
   fid << "e\n0 ";
   for (int i : m_patients[day]) {
      fid << get<0>(m_tw[i][day]) << " ";
   }
   fid << "0\n";

   fid << "l\n99999 ";
   for (int i : m_patients[day]) {
      fid << get<1>(m_tw[i][day]) << " ";
   }
   fid << "99999\n";


   // cout << "Instance of day " << day << " exported sucessfully.\n";
   cout << "File written: " << fidName << ".\n";

   return FileCode::WriteOk;
}

void WeeklyData::generateSkillPartitions() {
   m_skillPartitions[0] = make_tuple(0, m_params.numSkills()/2);
   m_skillPartitions[1] = make_tuple(m_params.numSkills() / 2, m_params.numSkills());

   m_carerPartitions[0] = make_tuple(0, m_params.numCaregivers() / 2);
   m_carerPartitions[1] = make_tuple(m_params.numCaregivers() / 2, m_params.numCaregivers());
}

void WeeklyData::generateCaregivers() {
   // Fills caregiver skills within these ranges.
   // This is just an approach to reduce code duplication.
   auto genSkillsRange = [&](tuple <int,int> skillRange, tuple <int,int> cgRange) {
      auto [si, sj] = skillRange;
      assert(sj-si >= 1 && "Invalid range of skills.");

      auto [ci, cj] = cgRange;
      assert(cj-ci >= 1 && "Invalid range of caregivers.");

      uniform_int_distribution <int> numSkillsDist(1, m_params.maxSkillsPerCaregiver());
      uniform_int_distribution <int> skillsDist(si, sj-1);

      for (int v = ci; v < cj; ++v) {
         // Chooses the number of skills to generate
         int count = accumulate(m_carerQualif[v].begin(), m_carerQualif[v].end(), 0);
         const int ns = max(0, numSkillsDist(m_params.prng()) - count);

         // Generate the skills.
         for (int sc = 0; sc < ns; ++sc) {
            // Samples the skills until choosing one that is not already set.
            int s = -1;
            do {
               s = skillsDist(m_params.prng());
            } while (caregiverHasSkill(v, s));

            // Update the data structures.
            setCaregiverSkill(v, s);
         }
      }
   };

   auto fillMissingSkills = [&](tuple <int,int> skillRange, tuple <int,int> cgRange) {
      auto [si, sj] = skillRange;
      assert(sj-si >= 1 && "Invalid range of skills.");

      auto [ci, cj] = cgRange;
      assert(cj-ci >= 1 && "Invalid range of caregivers.");

      // Shuffles the skills set.
      vector <int> skills(sj-si);
      iota(skills.begin(), skills.end(), si);
      shuffle(skills.begin(), skills.end(), m_params.prng());

      uniform_int_distribution <int> caregiverDist(ci, cj-1);

      for (int s: skills) {
         if (numCaregiversWithSkill(s) == 0) {
            for (;;) {
               int v = caregiverDist(m_params.prng());
               int count = accumulate(m_carerQualif[v].begin(), m_carerQualif[v].end(), 0);
               if (!caregiverHasSkill(v, s) &&
                   count < m_params.maxSkillsPerCaregiver()) {
                  setCaregiverSkill(v, s);
                  break;
               }
            }
         }
      }

   };

   fillMissingSkills(m_skillPartitions[0], m_carerPartitions[0]);
   fillMissingSkills(m_skillPartitions[1], m_carerPartitions[1]);

   genSkillsRange(m_skillPartitions[0], m_carerPartitions[0]);
   genSkillsRange(m_skillPartitions[1], m_carerPartitions[1]);
}

int WeeklyData::numCaregiversWithSkill(int skill) const {
   return m_carerPerSkill[skill];
}

bool WeeklyData::caregiverHasSkill(int caregiver, int skill) const {
   return m_carerQualif[caregiver][skill];
}

void WeeklyData::setCaregiverSkill(int caregiver, int skill) {
   assert(m_carerQualif[caregiver][skill] == 0 && "Caregiver already have this skill.");

   m_carerQualif[caregiver][skill] = 1;
   m_carerPerSkill[skill]++;
}

void WeeklyData::generatePatientsPool() {
   if (m_params.poolSizeOverride() == 0) {
      uniform_int_distribution<int> dist(
         m_params.minPatientsPerSkillUnit(),
         m_params.maxPatientsPerSkillUnit()
      );

      m_poolSize = 0;
      for (int s = 0; s < m_params.numSkills(); ++s) {
         m_poolSize += dist(m_params.prng()) * numCaregiversWithSkill(s);
      }
   } else {
      m_poolSize = m_params.numDailyPatients();
   }

   m_tw.resize(m_poolSize);
   for (auto &i: m_tw)
      i.resize(m_params.numDays(), {0, 0});

   m_patients.resize(m_params.numDays(), vector <int> ());

   m_demands.resize(m_poolSize);
   for (auto &i: m_demands) {
      i.resize(m_params.numDays());
      for (auto &j: i) {
         j.resize(m_params.numSkills(), 0);
      }
   }

   m_procTimes.resize(m_poolSize);
   for (auto &i: m_procTimes) {
      i.resize(m_params.numDays());
      for (auto &j: i) {
         j.resize(m_params.numSkills(), 0);
      }
   }

   m_separationTimes.resize(m_poolSize);
   for (auto &i: m_separationTimes)
      i.resize(m_params.numDays(), {0, 0});

   m_patientDays.resize(m_poolSize);
   for (auto &i: m_patientDays)
      i.resize(m_params.numDays(), 0);
}

void WeeklyData::generateDailyData(int day) {
   auto samplePatientsOfDay = [&]() {
      vector <int> patients(m_poolSize);
      iota(begin(patients), end(patients), 0);
      shuffle(begin(patients), end(patients), m_params.prng());

      patients.resize(m_params.numDailyPatients());
      m_patients[day] = patients;

      for (int i: patients)
         m_patientDays[i][day] = 1;
   };
   samplePatientsOfDay();

   auto generateTimeWindows = [&]() {
      uniform_int_distribution <int> twDist(
         m_params.dayStartingMinutes(),
         m_params.dayEndingMinutes() - m_params.twDurationMinutes()
      );
      for (int i: m_patients[day]) {
         const int beg = twDist(m_params.prng());
         m_tw[i][day] = {beg, beg + m_params.twDurationMinutes()};
      }
   };
   generateTimeWindows();

   // Used to generate demands proportionally to the number
   // of caregivers qualified to each each skill.
   discrete_distribution <int> demandDist(m_carerPerSkill.begin(), m_carerPerSkill.end());
   uniform_int_distribution <int> procTimeDist(m_params.minProcTime(), m_params.maxProcTime());

   // Caches the last required skill set to the patient.
   vector<int> demCache(m_poolSize, -1);

   auto generateSingleDemands = [&]() {
   for (int i: m_patients[day]) {
         const int s = demandDist(m_params.prng());

         m_demands[i][day][s] = 1;
         m_procTimes[i][day][s] = procTimeDist(m_params.prng());
         demCache[i] = s;
      }
   };
   generateSingleDemands();

   // Compute the number of double service patients.
   // Considers explicitly the proportion of simultaneous and precedence DS.
   // We use no rounding here. This harms the daily proportion of DS
   // patients on daily instances, and one may use a ceil function to
   // fix this behavior.
   int numSimult = ceil(m_params.simultaneousDoubleServicesPerc() * m_params.numDailyPatients());
   int numPred = ceil(m_params.precedenceDoubleServicesPerc() * m_params.numDailyPatients());

   vector <int> dsPatients = m_patients[day];
   shuffle(dsPatients.begin(), dsPatients.end(), m_params.prng());
   size_t dsPos = 0;

   auto generateDoubleDemands = [&](int sepMin, int sepMax, int count) {
      uniform_int_distribution <int> sepDist(sepMin, sepMax);

      for (int c = 0; c < count; ++c, ++dsPos) {
         int i = dsPatients[dsPos];
         bool cachedFirstHalf = demCache[i] < get<1>(m_skillPartitions[0]);
         int s = 0;
         for (;;) {
            s = demandDist(m_params.prng());
            bool otherFirstHalf = s < get<1>(m_skillPartitions[0]);
            if ((cachedFirstHalf == true && otherFirstHalf == false) ||
                (cachedFirstHalf == false && otherFirstHalf == true))
               break;
         }
         assert(accumulate(m_demands[i][day].begin(), m_demands[i][day].end(), 0) == 1);

         m_demands[i][day][s] = 1;
         m_procTimes[i][day][s] = procTimeDist(m_params.prng());
         demCache[i] = s;

         if (sepMax != 0) {
            int a = 0, b = 0;
            do {
               int v1 = sepDist(m_params.prng());
               int v2 = sepDist(m_params.prng());
               a = min(v1, v2);
               b = max(v1, v2);
            } while (b - a < m_params.minDynamicTwDurationMinutes());
            m_separationTimes[i][day] = {a, b};

         } else {
            m_separationTimes[i][day] = {0, 0};
         }
      }
   };

   generateDoubleDemands(0, 0, numSimult);
   generateDoubleDemands(m_params.minSeparation(), m_params.maxSeparation(), numPred);
}
