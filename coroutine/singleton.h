#ifndef COROUTINE_SINGLETON_H
#define COROUTINE_SINGLETON_H

// #include <list>
// #include <thread>
// #include <functional>
// #include <memory>
// #include <atomic>
#include <memory>
#include <utility>

namespace common
{

template <typename T>
class Singleton
{
  public:
    template <typename... Args>
    static T *Instance(Args &&... args)
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = new T(std::forward<Args>(args)...);
        }
        return m_pInstance;
    }

    static T *Instance(T *pInstance)
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = pInstance;
        }
        return m_pInstance;
    }

    static T *getInstance()
    {
        //https://stackoverflow.com/questions/20509734/null-vs-nullptr-why-was-it-replaced

        if (m_pInstance == nullptr)
        {
            throw std::logic_error("instance no init");
            // m_pInstance = new T(std::forward<Args>(args)...);
        }

        return m_pInstance;
    }

    static void DestroyInstance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

  private:
    Singleton(void);
    virtual ~Singleton(void);
    Singleton(const Singleton &);
    Singleton &operator=(const Singleton &);

  private:
    static T *m_pInstance;
};
template <class T>
T *Singleton<T>::m_pInstance = nullptr;
} // namespace common
#endif