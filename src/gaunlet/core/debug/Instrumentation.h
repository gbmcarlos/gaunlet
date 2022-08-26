#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <boost/current_function.hpp>

namespace gaunlet::Core {

    struct ProfileResult {
        std::string m_name;
        long long m_start, m_end;
    };

    struct InstrumentationSession {
        std::string m_name;
    };

    class Instrumentor {

    private:
        InstrumentationSession* m_currentSession;
        std::ofstream m_outputStream;
        int m_profileCount;

    public:
        Instrumentor() : m_currentSession(nullptr), m_profileCount(0) {}

        void beginSession(const std::string& name, const std::string& outputFilePath = "result.json") {
            m_outputStream.open(outputFilePath);
            writeHeader();
            m_currentSession = new InstrumentationSession({name});
        }

        void endSession() {
            writeFooter();
            m_outputStream.close();
            delete m_currentSession;
            m_currentSession = nullptr;
            m_profileCount = 0;
        }

        void writeProfile(const ProfileResult& result) {

            if (m_profileCount++ > 0) {
                m_outputStream << ",";
            }

            std::string name = result.m_name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_outputStream << "{";
            m_outputStream << "\"cat\": \"function\",";
            m_outputStream << "\"dur\":" << (result.m_end - result.m_start) << ",";
            m_outputStream << "\"name\": \"" << name << "\",";
            m_outputStream << "\"ph\": \"X\",";
            m_outputStream << "\"pid\": 0,";
            m_outputStream << "\"tid\": 0,";
            m_outputStream << "\"ts\": " << result.m_start;
            m_outputStream << "}";

            m_outputStream.flush();

        }

        static Instrumentor& get() {
            static Instrumentor m_instance;
            return m_instance;
        }

    private:
        void writeHeader() {
            m_outputStream << "{\"otherData\": {}, \"traceEvents\": [";
            m_outputStream.flush();
        }

        void writeFooter() {
            m_outputStream << "]}";
            m_outputStream.flush();
        }

    };

    class InstrumentationTimer {

    public:

        InstrumentationTimer(const char* name) : m_name(name), m_stopped(false) {
            m_startTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer() {

            if (!m_stopped) {
                stop();
            }

        }

        void stop() {

            auto endpoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endpoint).time_since_epoch().count();

            Instrumentor::get().writeProfile({m_name, start, end});

            m_stopped = true;

        }

    private:
        const char* m_name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
        bool m_stopped = false;

    };

}

#if 0
#define GE_PROFILE_BEGIN_SESSION(name, outputFilePath) ::gaunlet::Instrumentor::get().beginSession(name, outputFilePath);
#define GE_PROFILE_END_SESSION() ::gaunlet::Instrumentor::get().endSession();
#define GE_PROFILE_SCOPE(name) ::gaunlet::InstrumentationTimer timer##__LINE__(name);
#define GE_PROFILE_FUNCTION GE_PROFILE_SCOPE(BOOST_CURRENT_FUNCTION);
#else
#define GE_PROFILE_BEGIN_SESSION(name, outputFilePath)
#define GE_PROFILE_END_SESSION()
#define GE_PROFILE_SCOPE(name)
#define GE_PROFILE_FUNCTION
#endif