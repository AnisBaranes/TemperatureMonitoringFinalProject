#ifndef SRC_CLICOMMAND_H_
#define SRC_CLICOMMAND_H_


#include "CliCommand.h"
#include "project_init.h"
#include <string.h>
#include <stdio.h>

#define NAME_SIZE 50

class CliCommand {
public:
	virtual void doCommand(const char* param) = 0;
	virtual ~CliCommand() {}

};

class CliEntry {
public:
	char name[NAME_SIZE];
	CliCommand* cliCommand;
};


class Container {
private:
	CliEntry _cliCommand[20];
	int _counter;
public:
	void registerCommand(const char * name, CliCommand* clicommand){
		strncpy(_cliCommand[_counter].name, name, NAME_SIZE);
		_cliCommand[_counter].cliCommand = clicommand;
		_counter++;
	}
	void doCommand(const char* cmd, char * param){
		for (int i = 0; i < _counter; i++) {
				if (strcmp(_cliCommand[i].name, cmd) == 0) {
					_cliCommand[i].cliCommand->doCommand(param);
					return;
				}
			}
		printf("Invalid command\r\n");
	}

	void cliInit();

};

#endif
