#ifndef RWFILE_H
#define RWFILE_H

#include <QSettings>

#include "macroDefine.h"


#include "../robotServer/RobotStructure/robotStructure.h"

#define ROBOTDOF 6

void read_cache_ratio(QString&);
void write_cache_ratio(QString&);
void read_cache_program(QString&);
void write_cache_program(QString&);
void read_cache_sound(QString&);
void write_cache_sound(QString&);
void read_cache_language(QString&);
void write_cache_language(QString&);

void read_cache_Error(QString&);
void write_cache_Error(QString&);
#endif
