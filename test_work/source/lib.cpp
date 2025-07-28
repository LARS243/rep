#include "/home/g/Documents/test_work/include/lib.hpp"

int Journal::priority_to_int(std::string text_priority) {
    
    if (text_priority == "hight") {
        return 3;
    }
    else if (text_priority == "middle") {
        return 2;
    }
    else if (text_priority == "low") {
        return 1;
    }
    else { 
        
        return 0; 
    }
}

std::string Journal::get_time() {
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    std::string hours;
    std::string minuts;
    std::string seconds;

    if (std::to_string(now->tm_hour).size() == 1) {
        hours = "0" + std::to_string(now->tm_hour);
    }
    else {
        hours = std::to_string(now->tm_hour);
    }

    if (std::to_string(now->tm_min).size() == 1) { 
        minuts = "0" + std::to_string(now->tm_min);
    }
    else {
        minuts = std::to_string(now->tm_min);
    }

    if (std::to_string(now->tm_sec).size() == 1) { 
        seconds = "0" + std::to_string(now->tm_sec);
    }
    else {
        seconds = std::to_string(now->tm_sec);
    }

    return hours + ":" + minuts + ":" + seconds;
}
void Journal::add_element(std::string text, std::string text_priority) {
    
    if (priority_to_int(text_priority) >= priority_to_int(priority)) {
        std::string this_time = get_time();
        std::ofstream out(file_name + ".txt", std::ios::app);
        
        if (out.is_open()) {
            out << "time:" << this_time << "\npriority:" << text_priority << "\ntext:" << text << "\n==========================\n";
        }
        out.close();
    }
}
void Journal::set_priority(std::string new_priority){
    priority = new_priority;
}
void Journal::set_file_name(std::string new_file_name){
    file_name = new_file_name;
}
std::string Journal::get_priority(){
    return priority;
}