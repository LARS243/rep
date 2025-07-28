#ifndef JOURNAL_H 
#define JOURNAL_H

#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

class Journal 
{
private:
    std::string file_name;
    std::string priority = "low";
    int priority_to_int(std::string text_priority); 
    std::string get_time(); 
public:
    
    void add_element(std::string text, std::string text_priority);
    void set_priority(std::string new_priority);
    void set_file_name(std::string new_file_name);
    std::string get_priority();
};
#endif