#include "/home/g/Documents/test_work/include/lib.hpp"


class Interface {
private:
    Journal journal;
    std::string input = "";
    
    const std::string quit = "/quit";
    const std::string help = "/help";
    const std::string new_prior = "/jprior";
    const std::string write = "/write";
    const std::string get_prior = "/gprior";

    std::queue<std::pair<std::string, std::string>> write_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_condition;
    bool shutdown = false;
    std::thread writer_thread;

    void writer_worker() {
        while (true) {
            std::pair<std::string, std::string> item;
            
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                queue_condition.wait(lock, [this]() { 
                    return not write_queue.empty() or shutdown; 
                });
                
                if (shutdown and write_queue.empty()) break;
                
                item = write_queue.front();
                write_queue.pop();
            }
            
            journal.add_element(item.first, item.second);
        }
    }

    void print_help() {
        std::cout << "[/quit] to close the application\n";
        std::cout << "[/jprior] to change the journal priority\n";
        std::cout << "[/write] to write in journal\n";
        std::cout << "[/gprior] to get priority in journal\n";
    }

    void write_text() {
        std::string text;
        std::string text_prior;
        std::cout << "enter text: ";
        std::cin >> text;
        std::cout << "enter text priority: ";
        std::cin >> text_prior;
        while (text_prior != "low" and text_prior != "middle" and text_prior != "hight") {
            std::cout << "error. Use [low] or [middle] or [hight]";
            std::cout << "enter text priority: \n";
            std::cin >> text_prior;
        }
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            write_queue.emplace(text, text_prior);
        }
        queue_condition.notify_one();
    }

    void change_prior() {
        std::string new_prior;
        std::cout << "enter new priority: ";
        std::cin >> new_prior;
        if (new_prior == "low" or new_prior == "middle" or new_prior == "hight") {
            journal.set_priority(new_prior);
        }
        else {
            std::cout << "error. Use [low] or [middle] or [hight]";
        }
    }

    void check_input() {
        if (input.substr(0, help.size()) == help) {
            print_help();
        }
        else if (input.substr(0, new_prior.size()) == new_prior) {
            change_prior();
        }
        else if (input.substr(0, write.size()) == write) {
            write_text();
        }
        else if(input.substr(0, get_prior.size()) == get_prior) {
            std::cout << "[" << journal.get_priority() << "]\n";
        }
        else if (input.substr(0, quit.size()) == quit) {
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                shutdown = true;
            }
            queue_condition.notify_one();
            writer_thread.join();
        }
        else {
            std::cout << "Error.Invalid input. Please enter [/help]\n";
        }
    }

public:
    Interface(std::string input_file_name) : journal() {
        journal.set_file_name(input_file_name);
        writer_thread = std::thread(&Interface::writer_worker, this);
    }

    ~Interface() {
        if (writer_thread.joinable()) {
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                shutdown = true;
            }
            queue_condition.notify_one();
            writer_thread.join();
        }
    }

    void start_work() {
        do {
            std::cout << "enter command: ";
            std::cin >> input;
            check_input();
        } while(input != quit);
    }
};

int main() {
    std::string file;
    std::cout << "enter file name: ";
    std::cin >> file;
    Interface app(file);
    app.start_work();
    return 0;
}