#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <set>
#include "CourseModule.h"
#include "CourseGraph.h"


/**
 * @brief Represents a list of courses to be taken in a semester
 * @details The entire semester can be restricted from having courses
 * @details Certain courses can be restricted from being in this semester
*/
struct Semester {
	enum Seasons { WINTER, SPRING, SUMMER, FALL };

	Seasons season;
	int year;

	/**
	 * @brief The maximum number of units to take in this semester
	 * @details When maxUnits != 0, the maximum units for the schedule generator is overridden with this value
	*/
	int maxUnits = 0;

	/**
	 * @brief Courses being taken this semester
	*/
	std::vector<CourseModule> courses;

	/**
	 * @brief A vector containing courses that we cannot take in this semester
	*/
	std::vector<CourseModule> restricted;

	/**
	 * @brief If true, this semester is not allowed to have courses in it
	*/
	bool semesterRestricted = false;


	std::string ToString() const {
		std::string seasons[] = { "Winter", "Spring", "Summer", "Fall" };
		return seasons[static_cast<int>(season)] + " " + std::to_string(year);
	}

	const bool operator<(const Semester &s) const {
		if (year < s.year) return true;
		if (season < s.season) return true;
		return false;
	}
	const bool operator>(const Semester &s) const {
		if (year > s.year) return true;
		if (season > s.season) return true;
		return false;
	}
	const bool operator==(const Semester &s) const {
		return season == s.season && year == s.year;
	}
};

/**
 * @brief Represents all semesters of courses planned out contained in a vector of Semesters
*/
struct Schedule {
	std::vector<Semester> CoursePlan;
};


/**
 * @brief Used to generate schedules and allows for user to put restrictions on what courses can be taken when
 * @details Spring and Fall courses are automatically allowed while Winter and Summer are automatically not allowed. This can be changed with the setters
*/
class Scheduler {
private:

	CourseGraph Courses;
	std::set<CourseModule> completedCourses;

	std::vector<Semester> semsWithRestrictions;
	
	bool winterAllowed, springAllowed, summerAllowed, fallAllowed;

	int maxUnits;

	//Sets s to the next season allowed and if the season has to go to the next year, the year is incremented
	//Does not check whether any of the seasons are allowed. If that is the case, there will be an infinite loop
	void nextSemester(Semester::Seasons &s, int &y) const;

public:

	Scheduler();

	/**
	 * @brief Initializes the scheduler with a graph containing the courses and a list of completed courses
	 * @param g Graph containing all courses that need to be taken or have already been taken
	 * @param completed Vector containing all courses that have already been taken and don't need to be planned
	*/
	Scheduler(const CourseGraph &g, const std::set<CourseModule> &completed = std::set<CourseModule>());

	/**
	 * @brief Updates the list of courses we're generating schedules for
	 * @details Does not check if any courses have been removed so it does not remove those missing courses from semester restrictions
	 * @param g The new course graph
	*/
	void updateCourses(const CourseGraph &g);

	/**
	 * @brief Marks the course given as complete by removing it from the graph and adding it to the list of completed courses
	 * @param c Course completed
	*/
	void complete(const CourseModule &c);

	/**
	 * @brief Marks the courses given as complete by removing them from the graph and adding them to the list of completed courses
	 * @param vect Vector containing the courses we're marking as complete
	*/
	void complete(const std::vector<CourseModule> &vect);

	/**
	 * @brief Marks the complete course given as incomplete
	 * @param c Incomplete course
	*/
	void uncomplete(const CourseModule &c);

	/**
	 * @brief Marks the completed courses given as incomplete
	 * @param vect Vector containing the courses we're marking as incomplete
	*/
	void uncomplete(const std::vector<CourseModule> &vect);

	/**
	 * @brief Allows user to disallow a course from being taken in a particular semester
	 * @param c Course being restricted
	 * @param season Which semester of the year being restricted (Ex: Spring, Fall, etc.)
	 * @param year Year of the semester
	*/
	void addRestriction(const CourseModule &c, Semester::Seasons season, int year);

	/**
	 * @brief Removes the ability for this semester to contain any courses
	 * @param s Semester to be restricted
	*/
	void addRestriction(Semester s);

	/**
	 * @brief Removes the ability for this semester to contain any courses
	 * @param season Which semester (Ex: Spring, Fall, Winter, Summer) to be restricted
	 * @param year Year of the semester
	*/
	void addRestriction(Semester::Seasons season, int year);

	void setSemesterUnitLimit(Semester s, int lim);
	void setSemesterUnitLimit(Semester::Seasons season, int year, int lim);

	/**
	 * @brief Removes a restriction for a given semester
	 * @param c The restricted course
	 * @param season Which semester of the year was restricted
	 * @param year Year of the semester
	*/
	void removeRestriction(const CourseModule &c, Semester::Seasons season, int year);

	/**
	 * @brief Removes a restriction on a given semester
	 * @param season Which semester of the year was restricted
	 * @param year Year of the semester
	*/
	void removeRestriction(Semester::Seasons season, int year);

	/**
	 * @brief Sets an overall unit limit (default = 15)
	 * @details Minimum = 4
	 * @param lim The maximum units to be taken in any semester
	*/
	void setUnitLimit(int lim);

	Schedule generateSchedule(Semester::Seasons currentSeason, int currentYear) const;

	//Lets user determine if they want to take courses in particular semesters

	void setWinterAllowed(bool allowed);
	void setSpringAllowed(bool allowed);
	void setSummerAllowed(bool allowed);
	void setFallAllowed(bool allowed);

	const bool getWinterAllowed() const;
	const bool getSpringAllowed() const;
	const bool getSummerAllowed() const;
	const bool getFallAllowed() const;

	std::vector<Semester::Seasons> getRestrictedSeasons() const;

	std::vector<Semester> getRestrictedSemesters() const;
	
	int getMaxUnits() const;

};

#endif