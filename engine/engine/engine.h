//
// Copyright (C) 2016 Jonathon Love
//

#ifndef ENGINE_H
#define ENGINE_H

#include "engine.h"

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

#include "enginecoms.h"
#include "enginer.h"

class Engine
{
public:
    Engine();
    void setSlave(bool slave);
    void setConnection(const std::string &conn);
    void setPath(const std::string &path);
    void start();

private:
    void messageLoop();
    void monitorStdinLoop();
    void analysisRequested(int messageId, jamovi::coms::AnalysisRequest &request);
    void resultsReceived(const std::string &results, bool complete);
    void periodicChecks();
    void terminate();
    bool isNewAnalysisWaiting();

    EngineComs _coms;

    EngineR *_R;

    bool _slave;
    std::string _conString;
    std::string _path;
    int _socket;
    int _conId;
    bool _exiting;

    int _currentMessageId;
    jamovi::coms::AnalysisRequest _waitingRequest;
    jamovi::coms::AnalysisRequest _runningRequest;
    const google::protobuf::Reflection *_reflection;

    std::mutex _mutex;
    std::condition_variable _condition;
};

#endif // ENGINE_H
