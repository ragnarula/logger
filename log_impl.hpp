#ifndef LOG_IMPL_HPP
#define LOG_IMPL_HPP

#include <string>
#include <mutex>
#include <sstream>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>
#include <fstream>
#include <unistd.h>
#include <libproc.h>

namespace logging {
enum severity_type
{
    debug = 1,
    error,
    warning
};

class log_policy_interface
{
public:
    virtual void     setup() = 0;
    virtual void     teardown() = 0;
    virtual void     write(const std::string& msg) = 0;
    virtual ~log_policy_interface() {}
};

class file_log_policy : public log_policy_interface
{
    std::string name;
    std::shared_ptr<std::ofstream> out_stream;
    std::string dir;
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

    const char* getProcPath()
    {
        pid_t pid = getpid();
        proc_pidpath (pid, file_log_policy::pathbuf, sizeof(file_log_policy::pathbuf));
        return &pathbuf[0];
    }

public:
    file_log_policy(const std::string& _name) :
        name(_name),
        out_stream( new std::ofstream )
    {
        dir = getProcPath();
        std::string::size_type pos;
        if((pos = dir.find(".app",0,4)) == std::string::npos){
            dir.erase(dir.begin()+dir.find_last_of('/')+1, dir.end());
        } else {
            dir.erase(dir.begin()+pos+5, dir.end());
        }
    }
    void setup()
    {
        try{
            out_stream->open( (dir+name).c_str(), std::ios_base::binary|std::ios_base::out );
        } catch (std::exception) {
            teardown();
        }
    }
    void teardown()
    {
        if( out_stream ){
            out_stream->close();
        }
    }
    void write(const std::string& msg)
    {
        if(!out_stream->is_open())
            return;
        (*out_stream)<<msg<<std::endl;
    }
    ~file_log_policy()
    {
        if( out_stream ){
            teardown();
        }
    }
};

class logger
{
    std::stringstream log_stream;
    std::mutex write_mutex;
    unsigned log_line_number;
    std::vector<log_policy_interface*> policy;

    std::string get_time()
    {
        std::string time_str;
        time_t raw_time;
        time( & raw_time );
        time_str = ctime( &raw_time );
        //without the newline character
        return time_str.substr( 0 , time_str.size() - 1 );
    }
    std::string get_logline_header()
    {
        std::stringstream header;
        header.str("");
        header.fill('0');
        header.width(7);
        header << log_line_number++ <<" < "<<get_time()<<" - ";
        header.fill('0');
        header.width(7);
        header <<clock()<<" > ~ ";
        return header.str();
    }

    //Core printing functionality
    void print_impl()
    {
        for(std::vector<log_policy_interface* >::iterator iter = policy.begin(); iter != policy.end(); iter++){
            (*iter)->write( get_logline_header() + log_stream.str() );
        }

        log_stream.str("");
    }

    template<typename First, typename...Rest >
    void print_impl(First parm1, Rest...parm)
    {
        log_stream<<parm1;
        print_impl(parm...);
    }


public:
    logger()
    {
        log_line_number = 0;
    }
    ~logger()
    {
        for(std::vector<log_policy_interface* >::iterator iter = policy.begin(); iter != policy.end(); iter++){
            (*iter)->teardown();
            delete *iter;
        }
    }

    template < severity_type severity , typename...Args >
    void print( Args...args )
    {
        write_mutex.lock();
        switch( severity )
        {
        case severity_type::debug:
            log_stream<<"<DEBUG> :";
            break;
        case severity_type::warning:
            log_stream<<"<WARNING> :";
            break;
        case severity_type::error:
            log_stream<<"<ERROR> :";
            break;
        };
        print_impl( args... );
        write_mutex.unlock();
    }
    void register_policy(log_policy_interface *policy_)
    {
        policy_->setup();
        policy.push_back(policy_);
    }
};
} //namespace logging


#endif // LOG_HPP
