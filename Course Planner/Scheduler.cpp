#include <queue>
#include "Scheduler.h"

void Scheduler::incrementSeason(Semester::Seasons &s, int &y) {
	bool foundSeason = false;
	Semester::Seasons formerSeason(s);
	while (!foundSeason) {

		if (static_cast<int>(s) == 3) ++y;
		s = static_cast<Semester::Seasons>((static_cast<int>(s) + 1) % 4);

		if (s == Semester::WINTER && winterAllowed) foundSeason = true;
		else if (s == Semester::SPRING && springAllowed) foundSeason = true;
		else if (s == Semester::SUMMER && summerAllowed) foundSeason = true;
		else if (s == Semester::FALL && fallAllowed) foundSeason = true;
	}


}

Scheduler::Scheduler(const CourseGraph &g, const std::set<CourseModule> &completed) {
	Courses = g;
	completedCourses = completed;
	for (auto &i : completedCourses) {
		Courses.remove(i);
	}

	springAllowed = true;
	fallAllowed = true;
	winterAllowed = false;
	summerAllowed = false;

	maxUnits = 15;
}

void Scheduler::complete(const CourseModule &c) {
	completedCourses.insert(c);
	Courses.remove(c);
}

void Scheduler::complete(const std::vector<CourseModule> &vect) {
	for (const auto &i : vect) {
		completedCourses.insert(i);
		Courses.remove(i);
	}
}

void Scheduler::addRestriction(const CourseModule &c, Semester::Seasons season, int year) {
	Semester sem;
	sem.season = season;
	sem.year = year;

	std::vector<Semester>::iterator semIt = std::find(semsWithRestrictions.begin(), semsWithRestrictions.end(), sem);
	if (semIt != semsWithRestrictions.end()) sem = *semIt;

	sem.restricted.push_back(c);
	semsWithRestrictions.push_back(sem);
}

void Scheduler::removeRestriction(const CourseModule &c, Semester::Seasons season, int year) {
	Semester sem;
	sem.season = season;
	sem.year = year;

	std::vector<Semester>::iterator resSem = std::find(semsWithRestrictions.begin(), semsWithRestrictions.end(), sem);
	if (resSem == semsWithRestrictions.end()) return;

	std::vector<CourseModule>::iterator courseLocation = std::find(resSem->restricted.begin(), resSem->restricted.end(), c);
	if (courseLocation == resSem->restricted.end()) return;

	resSem->restricted.erase(courseLocation);
}

void Scheduler::setUnitLimit(int lim, Semester::Seasons season, int year) {
	Semester sem;
	sem.season = season;
	sem.year = year;

	std::vector<Semester>::iterator semIt = std::find(semsWithRestrictions.begin(), semsWithRestrictions.end(), sem);
	if (semIt != semsWithRestrictions.end()) sem = *semIt;

	sem.maxUnits = lim;
}

void Scheduler::setUnitLimit(int lim) {
	maxUnits = lim;
}

Schedule Scheduler::generateSchedule(Semester::Seasons currentSeason, int currentYear) {
	//Return an empty schedule if there are no allowed seasons
	if (!(winterAllowed || springAllowed || summerAllowed || fallAllowed)) return Schedule();

	CourseGraph remainingCourses(Courses);
	Schedule schedule;

	std::vector<Semester>::iterator restrictedSem = semsWithRestrictions.begin();

	//Queue containing leaf courses that were not assigned to the last semester due to restrictions
	std::queue<CourseModule> queuedCourses;
	while (!remainingCourses.empty()) {

		for (auto &i : remainingCourses) {
			if (i->prerequisites.empty()) {
				queuedCourses.push(i->course);
			}
		}

		//Removing the courses from the list
		std::queue<CourseModule> toBeRemoved(queuedCourses);
		while (!toBeRemoved.empty()) {
			remainingCourses.remove(toBeRemoved.front());
			toBeRemoved.pop();
		}


		int unitsSum = 0;
		Semester currentSem;
		if (restrictedSem != semsWithRestrictions.end()) {
			if (currentSeason == restrictedSem->season && currentYear == restrictedSem->year) {
				currentSem = *restrictedSem;
				++restrictedSem;
			}
		}
		int maxSemUnits = (currentSem.maxUnits > 0) ? currentSem.maxUnits : maxUnits;

		while (unitsSum <= maxSemUnits || !queuedCourses.empty()) {
			currentSem.courses.push_back(queuedCourses.front());
			queuedCourses.pop();
		}

		schedule.CoursePlan.push_back(currentSem);
		incrementSeason(currentSeason, currentYear);
	}

	return schedule;
}

void Scheduler::setWinterAllowed(bool allowed) {
	winterAllowed = allowed;
}

void Scheduler::setSpringAllowed(bool allowed) {
	springAllowed = allowed;
}

void Scheduler::setSummerAllowed(bool allowed) {
	summerAllowed = allowed;
}

void Scheduler::setFallAllowed(bool allowed) {
	fallAllowed = allowed;
}

std::vector<Semester::Seasons> Scheduler::getRestrictedSeasons() {
	std::vector<Semester::Seasons> v;
	if (!winterAllowed) v.push_back(Semester::WINTER);
	if (!springAllowed) v.push_back(Semester::SPRING);
	if (!summerAllowed) v.push_back(Semester::SUMMER);
	if (!fallAllowed) v.push_back(Semester::FALL);

	return v;
}

std::vector<Semester> Scheduler::getRestrictedSemesters() {
	return semsWithRestrictions;
}

int Scheduler::getMaxUnits() {
	return maxUnits;
}


