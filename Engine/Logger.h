#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <cassert>

enum class LogLevel { LOG_INFO, LOG_WARNING, LOG_ERROR };

class LogSink {
public:
    virtual void log(LogLevel level, const std::string& message) = 0;
    virtual ~LogSink() = default;
};

class ConsoleSink : public LogSink {
public:
    void log(LogLevel level, const std::string& message) override {
        std::cout << logLevelToString(level) << " " << message << std::endl;
    }

private:
    std::string logLevelToString(LogLevel level) {
        switch (level) {
        case LogLevel::LOG_INFO: return "[INFO]";
        case LogLevel::LOG_WARNING: return "[WARNING]";
        case LogLevel::LOG_ERROR: return "[ERROR]";
        default: return "[UNKNOWN]";
        }
    }
};

class FileSink : public LogSink {
private:
    std::ofstream logFile;

public:
    FileSink(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }

    void log(LogLevel level, const std::string& message) override {
        if (logFile) {
            logFile << logLevelToString(level) << " " << message << std::endl;
        }
    }

    ~FileSink() {
        if (logFile.is_open()) logFile.close();
    }

private:
    std::string logLevelToString(LogLevel level) {
        switch (level) {
        case LogLevel::LOG_INFO: return "[INFO]";
        case LogLevel::LOG_WARNING: return "[WARNING]";
        case LogLevel::LOG_ERROR: return "[ERROR]";
        default: return "[UNKNOWN]";
        }
    }
};

class Logger {
private:
    std::vector<std::shared_ptr<LogSink>> sinks;
    std::mutex logMutex;

public:
    void addSink(std::shared_ptr<LogSink> sink) {
        if (!sink) {
            throw std::invalid_argument("Cannot add null sink to logger");
        }
        sinks.push_back(sink);
    }

    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        for (auto& sink : sinks) {
            if (sink) {
                sink->log(level, message);
            }
        }
    }

    void info(const std::string& message) { log(LogLevel::LOG_INFO, message); }
    void warn(const std::string& message) { log(LogLevel::LOG_WARNING, message); }
    void error(const std::string& message) { log(LogLevel::LOG_ERROR, message); }
};

class LoggerRegistry {
private:
    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;
    std::shared_ptr<Logger> defaultLogger;
    std::mutex registryMutex;

    LoggerRegistry() {
        defaultLogger = std::make_shared<Logger>();
        defaultLogger->addSink(std::make_shared<ConsoleSink>());
    }

public:
    static LoggerRegistry& getInstance() {
        static LoggerRegistry instance;
        return instance;
    }

    std::shared_ptr<Logger> getLogger(const std::string& name) {
        std::lock_guard<std::mutex> lock(registryMutex);
        auto it = loggers.find(name);
        if (it != loggers.end()) {
            return it->second;
        }
        return defaultLogger;
    }

    void setDefaultLogger(std::shared_ptr<Logger> logger) {
        if (!logger) {
            throw std::invalid_argument("Cannot set null default logger");
        }
        defaultLogger = logger;
    }

    void registerLogger(const std::string& name, std::shared_ptr<Logger> logger) {
        if (!logger) {
            throw std::invalid_argument("Cannot register null logger for: " + name);
        }
        std::lock_guard<std::mutex> lock(registryMutex);
        loggers[name] = logger;
    }
};

#define LOG_INFO(message) LoggerRegistry::getInstance().getLogger("global")->info(message)
#define LOG_WARN(message) LoggerRegistry::getInstance().getLogger("global")->warn(message)
#define LOG_ERROR(message) LoggerRegistry::getInstance().getLogger("global")->error(message)

#define ASSERT_MSG(condition, message) \
    do { \
        if (!(condition)) { \
            LOG_ERROR(std::string("ASSERT FAILED: ") + message); \
            assert(condition && message); \
        } \
    } while(0)

#define ASSERT(condition) ASSERT_MSG(condition, #condition)