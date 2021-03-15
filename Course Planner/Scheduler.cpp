#include <queue>
#include "Scheduler.h"

void Scheduler::nextSemester(Semester::Seasons &s, int &y) const {
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

Scheduler::Scheduler() {
	Courses = CourseGraph();
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

void Scheduler::updateCourses(const CourseGraph &g) {
	Courses = g;
}

void Scheduler::complete(const CourseModule &c) {
	completedCourses.insert(c);
}

void Scheduler::complete(const std::vector<CourseModule> &vect) {
	for (const auto &i : vect) {
		completedCourses.insert(i);
	}
}

void Scheduler::uncomplete(const CourseModule &c) {
	if (completedCourses.find(c) != completedCourses.end()) completedCourses.erase(c);
}

void Scheduler::uncomplete(const std::vector<CourseModule> &vect) {
	for (const auto &i : vect) {
		if (completedCourses.find(i) != completedCourses.end()) completedCourses.erase(i);
	}
}

void Scheduler::addRestriction(const CourseModule &c, Semester::Seasons season, int year) {
	Semester sem;
	sem.season = season;
	sem.year = year;

	std::vector<Semester>::iterator semIt = std::find(semsWithRestrictions.begin(), semsWithRestrictions.end(), sem);
	if (semIt != semsWithRestrictions.end()) {
		semIt->restricted.push_back(c);
	}
	else {
		sem.restricted.push_back(c);
		semsWithRestrictions.push_back(sem);
	}
}

void Scheduler::addRestriction(Semester s) {
	addRestriction(s.season, s.year);
}

void Scheduler::addRestriction(Semester::Seasons season, int year) {
	Semester sem;
	sem.season = season;
	sem.year = year;

	std::vector<Semester>::iterator semIt = std::find(semsWithRestrictions.begin(), semsWithRestrictions.end(), sem);
	if (semIt != semsWithRestrictions.end()) {
		semIt->semesterRestricted = true;
	}
	else {
		sem.semesterRestricted = true;
		semsWithRestrictions.push_back(sem);
	}
}

void Scheduler::setSemesterUnitLimit(Semester s, int lim) {
	std::vector<Semester>::iterator semIt = std::find(semsWithRestrictions.begin(), semsWithRestrictions.end(), s);
	if (semIt != semsWithRestrictions.end()) {
		semIt->maxUnits = (lim >= 4 || lim == 0) ? lim : 4;
	}
	else if (lim) {
		s.maxUnits = (lim >= 4 || lim == 0) ? lim : 4;
		semsWithRestrictions.push_back(s);
	}

		if (lim == 0 && semIt->restricted.empty() && !semIt->semesterRestricted) //Semester no longer has restrictions
			semsWithRestrictions.erase(semIt);
}

void Scheduler::setSemesterUnitLimit(Semester::Seasons season, int year, int lim) {
	Semester sem;
	sem.season = season;
	sem.year = year;
	setSemesterUnitLimit(sem, lim);
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

	if (resSem->restricted.empty() && !resSem->semesterRestricted && maxUnits == 0) //Semester no longer has restrictions
		semsWithRestrictions.erase(resSem);
}

void Scheduler::removeRestriction(Semester::Seasons season, int year) {
	Semester sem;
	sem.season = season;
	sem.year = year;

	std::vector<Semester>::iterator resSem = std::find(semsWithRestrictions.begin(), semsWithRestrictions.end(), sem);
	if (resSem == semsWithRestrictions.end()) return;

	resSem->semesterRestricted = false;
	if (resSem->restricted.empty() && !resSem->semesterRestricted && maxUnits == 0) //Semester no longer has restrictions
		semsWithRestrictions.erase(resSem);
}

void Scheduler::setUnitLimit(int lim) {
	if (lim == 0) maxUnits = 15;
	else maxUnits = (lim >= 4) ? lim : 4;
}

Schedule Scheduler::generateSchedule(Semester::Seasons currentSeason, int currentYear) const {
	//Return an empty schedule if there are no allowed seasons
	if (!(winterAllowed || springAllowed || summerAllowed || fallAllowed)) return Schedule();

	CourseGraph remainingCourses(Courses);
	for (const auto &i : completedCourses) {
		remainingCourses.remove(i);
	}
	Schedule schedule;

	//Queue containing leaf courses that were not assigned to the last semester due to restrictions
	std::queue<CourseModule> queuedCourses;

	while (!remainingCourses.empty()) {
		Semester currentSem;
		currentSem.season = currentSeason;
		currentSem.year = currentYear;

		std::vector<Semester>::const_iterator restrictedSem = semsWithRestrictions.begin();
		while (restrictedSem != semsWithRestrictions.end()) {
			if (restrictedSem->season == currentSem.season && restrictedSem->year == currentSem.year)
				break;
			++restrictedSem;
		}
		if (restrictedSem != semsWithRestrictions.end()) {
			currentSem = *restrictedSem;
		}
		int maxSemUnits = (currentSem.maxUnits > 0) ? currentSem.maxUnits : maxUnits;

		std::vector<vertex *> leaves;
		for (auto &i : remainingCourses) {
			if (i->prerequisites.empty()) {
				if (std::find(currentSem.restricted.begin(), currentSem.restricted.end(), i->course) == currentSem.restricted.end()) { //Checking if course is not restricted for this semester
					leaves.push_back(i);
				}
			}
		}

		//Insertion sort to sort the leaf courses by highest outdegree
		//We do this to get courses that are more important out of the way first so we can take more courses the next semester
		for (int i = 1; i < leaves.size(); ++i) {
			vertex *key = leaves[i];
			int j = i - 1;
			while (j >= 0 && leaves[j]->prerequisiteFor.size() < key->prerequisiteFor.size()) {
				leaves[j + 1] = leaves[j];
				--j;
			}
			leaves[j + 1] = key;
		}


		int sum = 0;
		for (const auto &i : leaves) {
			if (sum + i->course.getUnits() <= maxSemUnits) {
				queuedCourses.push(i->course);
				sum += i->course.getUnits();
				remainingCourses.remove(i->course); //Remove course from the graph
			}
		}

		if (!currentSem.semesterRestricted) { //If this semester is allowed to have courses

			while (!queuedCourses.empty()) { //Add courses from the queue into the current semester
				if (std::find(currentSem.restricted.begin(), currentSem.restricted.end(), queuedCourses.front()) == currentSem.restricted.end()) {
					currentSem.courses.push_back(queuedCourses.front());
					queuedCourses.pop();
				}
			}

			schedule.CoursePlan.push_back(currentSem); //Push the semester to the schedule
		}
		
		nextSemester(currentSeason, currentYear);
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

const bool Scheduler::getWinterAllowed() const { return winterAllowed; }

const bool Scheduler::getSpringAllowed() const { return springAllowed; }

const bool Scheduler::getSummerAllowed() const { return summerAllowed; }

const bool Scheduler::getFallAllowed() const { return fallAllowed; }

std::vector<Semester::Seasons> Scheduler::getRestrictedSeasons() const {
	std::vector<Semester::Seasons> v;
	if (!winterAllowed) v.push_back(Semester::WINTER);
	if (!springAllowed) v.push_back(Semester::SPRING);
	if (!summerAllowed) v.push_back(Semester::SUMMER);
	if (!fallAllowed) v.push_back(Semester::FALL);

	return v;
}

std::vector<Semester> Scheduler::getRestrictedSemesters() const {
	return semsWithRestrictions;
}

int Scheduler::getMaxUnits() const {
	return maxUnits;
}


