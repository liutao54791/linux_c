#ifndef CMCC_H
#define CMCC_H

#include <stdint.h>
#include <string>

class cmcc_info
{
public:
    cmcc_info()
    {};
    int cmcc_string;
    int cmcc_value;
    static cmcc_info* getInstance()
    {
        static cmcc_info* m_instance;
        if (m_instance == NULL)
        {
            m_instance = new cmcc_info();
            printf("the first time to get instance\n");
        }
    
        return m_instance;
    };

    void cmcc_get_value(void);
    ~cmcc_info()
    {
    };
};

#endif