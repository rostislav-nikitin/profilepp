#ifndef __PROFILEPP_HPP__
#define __PROFILEPP_HPP__

#include <iostream>
#include <string>
#include <chrono>
#include <fstream>

#ifndef PROFILING_ENABLED
	#define PROFILING_ENABLED 1
#endif

#if PROFILING_ENABLED == 1
	#define PROFILE(name) Profile __temp_profile__##__LINE__##__(name)
#else
	#define PROFILE(name)
#endif

#if defined(_WIN32) || defined(WIN32)
	#define PROFILE_FN() PROFILE(__FUNCSIG__)
#else
	#define PROFILE_FN() PROFILE(__PRETTY_FUNCTION__)
#endif

class ProfileResult
{
public:
	ProfileResult(
		std::string name,
		std::chrono::time_point<std::chrono::steady_clock> start, 
		std::chrono::time_point<std::chrono::steady_clock> end) : name_(name), start_(start), end_(end)
	{
	}

	std::string getName() const { return name_; }
	std::chrono::time_point<std::chrono::steady_clock> getStart() const { return start_; }
	std::chrono::time_point<std::chrono::steady_clock> getEnd() const { return end_; }
	
private:
	std::string name_;
	std::chrono::time_point<std::chrono::steady_clock> start_;
	std::chrono::time_point<std::chrono::steady_clock> end_;
};

class ProfileSession
{
public:


	static ProfileSession* getInstance()
	{
		if(instance_ == nullptr)
			instance_ = new ProfileSession();

		return instance_;
	}

	void begin()
	{
		output_.open("profile.json");
		output_ << "{\"otherData\": {}, \"traceEvents\":[";
		output_.flush();
	}
	void end()
	{
		output_ << "]}";
		output_.flush();
		output_.close();
	}
	void addProfile(ProfileResult const &profileResult)
	{
		if (count_ > 0)
			output_ << ", ";

		output_
			<< "{"
			<< "\"cat\":\"function\", "
			<< "\"dur\":" << std::chrono::duration_cast<std::chrono::microseconds>(profileResult.getEnd() - profileResult.getStart()).count() << ", "
			<< "\"name\":\"" << profileResult.getName() << "\", "
			<< "\"ph\":\"X\", "
			<< "\"pid\":0, "
			<< "\"tid\":0, "
			<< "\"ts\":" << std::chrono::duration_cast<std::chrono::microseconds>(profileResult.getStart().time_since_epoch()).count()
			<< "}";
		output_.flush();

		++count_;
			
	}
private:
	ProfileSession() = default;
	static ProfileSession *instance_;
	
	std::ofstream output_;
	int count_ = 0;
};

ProfileSession *ProfileSession::instance_ = nullptr;


class Profile
{
public:
	Profile(std::string name) : name_(name), started_(std::chrono::steady_clock::now())
	{
	}

	~Profile()
	{
		finished_ = std::chrono::steady_clock::now();

		auto duration = finished_ - started_;
		ProfileSession::getInstance()->addProfile(ProfileResult(name_, started_, finished_));
		//std::cout << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << std::endl;
	}
private:
	std::string name_;
	std::chrono::time_point<std::chrono::steady_clock> started_;
	std::chrono::time_point<std::chrono::steady_clock> finished_;
};

#endif
