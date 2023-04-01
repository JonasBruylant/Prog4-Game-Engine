#pragma once
class Subject
{
public:
	Subject();

	~Subject();
	Subject(const Subject& other) = delete;
	Subject(Subject&& other) = delete;
	Subject& operator=(const Subject& other) = delete;
	Subject& operator=(Subject&& other) = delete;


private:
};

