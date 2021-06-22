#pragma once

#include "GeneratorParameters.hpp"

#include <array>
#include <random>
#include <tuple>
#include <vector>

class WeeklyData {
public:
   WeeklyData(GeneratorParameters &params);
   virtual ~WeeklyData();

   int generateInitialData();

   // Query the number of locations to generate.
   int patientsPoolSize() const;

   void setLocations(const std::vector <GeoLocation> &locs);
   void setDistance(int origin, int dest, double dist);

   void generateDailyData();
   FileCode writeDailyData(int day) const;

private:
   GeneratorParameters &m_params;

   // Partitioning of skills set.
   // Used when sampling skills for caregivers and
   // demand requests of the patients.
   std::array <std::tuple <int, int>, 2> m_skillPartitions;

   // Partitioning of caregivers set.
   // Used when sampling skills for caregivers and
   // demand requests of the patients.
   std::array <std::tuple <int, int>, 2> m_carerPartitions;

   // [caregiver][skill] -> 0 or 1
   std::vector <std::vector <int>> m_carerQualif;

   // [skill] -> number of caregivers
   std::vector <int> m_carerPerSkill;

   // Number of patients in the pool.
   int m_poolSize;

   // Patient tw.
   // [patient][day] -> 0:twMin 1:twMax
   std::vector <std::vector <std::tuple <int, int>>> m_tw;

   // Patients selected for each day of the planning horizon.
   // [day] -> list of patients (sampled from the pool)
   std::vector <std::vector <int>> m_patients;

   // [patient][day][skill] -> 0 or 1
   std::vector<std::vector<std::vector <int>>> m_demands;

   // [patient][day][skill] -> proc time in minutes
   std::vector <std::vector <std::vector <int>>> m_procTimes;

   // [patient][day] -> 0:deltaMin 1:deltaMax
   std::vector <std::vector <std::tuple <int, int>>> m_separationTimes;

   // Global counter of how many days a patient is allocated.
   // This bitmap register demands, and days which the patients
   // has a time window set.
   // [patient][day] -> 0 or 1.
   std::vector <std::vector <int>> m_patientDays;

   // Location of the depot, and each patient.
   // First element is the depot. The subsequent elements are the patients
   // from the pool.
   std::vector<GeoLocation> m_locations;

   // Travel times matrix. All values are in minutes.
   std::vector <std::vector <double>> m_travelTimes;

   void generateSkillPartitions();
   void generateCaregivers();

   int numCaregiversWithSkill(int skill) const;

   bool caregiverHasSkill(int caregiver, int skill) const;
   void setCaregiverSkill(int caregiver, int skill);

   void generatePatientsPool();
   void generateLocationsAndTravelTimes();

   void generateDailyData(int day);
};
