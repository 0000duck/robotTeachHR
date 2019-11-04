#include "rwFile.h"

using namespace std;
using namespace rclib;

void read_cache_program(QString& program){
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    program = st.value("user/programName").toString();
}

void write_cache_program(QString& m_program){
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    st.setValue("user/programName", m_program);
}

void read_cache_ratio(QString& ratio){
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    ratio = st.value("user/paintRatio").toString();
}

void write_cache_ratio(QString& m_ratio){
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    st.setValue("user/paintRatio", m_ratio);
}

void read_cache_sound(QString& sound)
{
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    sound = st.value("user/Sound").toString();
}

void write_cache_sound(QString& m_sound)
{
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    st.setValue("user/Sound", m_sound);
}

void read_cache_language(QString& language)
{
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    language = st.value("user/Language").toString();
}

void write_cache_language(QString& m_languahe)
{
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    st.setValue("user/Language", m_languahe);
}

void read_cache_Error(QString &error)
{
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    error = st.value("user/ProgramError").toString();
}

void write_cache_Error(QString &m_error)
{
    QSettings st(WINDOWCACHECONFIGPATH_INI, QSettings::IniFormat);
    st.setValue("user/ProgramError", m_error);
}
