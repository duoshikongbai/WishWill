#ifndef __DO_THREAD_HPP__
#define __DO_THREAD_HPP__
#include <thread>
#include <iostream>

template <class Task, class Core = std::thread>
class DoThread {
public:
    DoThread(Task* pTask) {
        m_pTask = pTask;
        m_Core = std::make_shared<Process>(DoThread::DoWorker, this);
    }
    
    DoThread() = delete;
    DoThread(const DoThread& process) = delete;
    DoThread& operator=(const DoThread& process) = delete;

    ~DoThread() {
        StopProcess();
        if(nullptr != m_pTask) {
            delete m_pTask;
        }
    }

    DoThread(const DoThread&& unit) {
        m_Core = std::move(unit.m_Core);
        m_pTask = std::move(unit.m_pTask);
    }

    DoThread& operator=(const DoThread&& unit) {

        DoThread(std::move(unit)).Swap(*this);
        return *this;
    }

    static void DoWorker(DoThread* pArgs) {

        if(nullptr != pArgs && nullptr != pArgs->m_pTask) {
            pArgs->m_pTask->DoWorker();
        }

        return ;
    }

    void Swap(DoThread& unit) {
        std::swap(m_pTask, unit.m_pTask);
        std::swap(m_Core, unit.m_Core);
    }

    int GetThreadId() {
        return m_Core->get_id();
    }

    Task* GetTaskPtr() {
        return m_pTask;
    }

    void StopProcess() {

        m_pTask->Stop();

        if(m_Core->joinable()) {
            m_Core->join();
        }
    }

private:

    Task* m_pTask = {nullptr};
    std::shared_ptr<Core> m_Core;
};


#endif