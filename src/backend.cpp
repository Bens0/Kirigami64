#include "backend.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <dirent.h>

char* string_to_char(std::string inp) {
	char* charOut = new char[inp.length() + 1];
	strcpy(charOut, inp.c_str());
	return charOut;
}

Backend::Backend(QObject *parent)
	: QObject(parent)
{
	buildFind(0);
}

int Backend::buildCountValue() {
	return buildCount;
}

int Backend::buildSelectedValue() {
	return buildSelected;
}

QString Backend::buildList(int pos) {
	return builds[pos];
}

QString Backend::repoText()
{
	return repo;
}

QString Backend::branchText()
{
	return branch;
}

bool Backend::downloadSizeUnknownValue() {
	return downloadSizeUnknown;
}

void Backend::buildFind(int additive) {
	int count = 0 + additive;
	int i = 0;
	DIR *dir;
	struct dirent *dirEntry;
	builds->clear();
	if ((dir = opendir ("sm64-builds")) != NULL) {
		while ((dirEntry = readdir (dir)) != NULL) {
			printf("Directory found: %s\n", dirEntry->d_name);
			if (dirEntry->d_type == DT_DIR && dirEntry->d_name[0] != '.') {
				builds[i] = dirEntry->d_name;
				count++;
				i++;
			}
		}
		printf("builds[0] is: %s\n", builds[0]);
		closedir (dir);
	} else {
		perror ("");
	}
	buildCount = count;
	Q_EMIT buildCountModified();
	Q_EMIT buildListModified();
}

void Backend::setBuildSelected(int target) {
	buildSelected = target;
	Q_EMIT buildSelectModified();
}

void Backend::buildListSet(QString &folder) {
	// This does nothing. It just emits buildListModified().
	Q_EMIT buildListModified();
}

void Backend::setRepo(QString &repoInp)
{
	repo = repoInp;
	Q_EMIT repoModified();
}

void Backend::setBranch(QString &branchInp)
{
	branch = branchInp;
	Q_EMIT branchModified();
}

void Backend::setDownloadSizeUnknown(bool &known)
{
	downloadSizeUnknown = known;
	Q_EMIT downloadSizeUnknownModified();
}

int Backend::clone() {
	if (!opendir("sm64-builds/")) {
		mkdir("sm64-builds",0777);
	}
	//std::string stdFolder = "sm64-builds/";
	std::string command = "git clone --branch " + branch.toStdString() + " " + repo.toStdString() + " --progress > .out";
	/*const char *dir = (stdFolder).c_str();
	mkdir(dir,0777);*/
	std::string cmd = "cd sm64-builds && " + command + " &";
	system(string_to_char(cmd));
	buildFind(0);
	return 0;
}

int Backend::pull(QString folder) {
	std::string cmdAsString = "cd sm64-builds/" + folder.toStdString() + " && git pull &";
	system(string_to_char(cmdAsString));
	return 0;
}