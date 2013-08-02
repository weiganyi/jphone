/**********************************************************
* Author: weiganyi
* File: JSingleton_T.cpp
* Date: 20121105
* Description: 
*
**********************************************************/

#ifndef _JSINGLETON_T_CPP_
#define _JSINGLETON_T_CPP_


template<class TYPE> TYPE* JSingleton<TYPE>::m_pObject = JNULL;

template<class TYPE> JSingleton<TYPE>::JSingleton()
{
    return;
}

template<class TYPE> JSingleton<TYPE>::~JSingleton()
{
    return;
}

template<class TYPE> TYPE* JSingleton<TYPE>::instance()
{
    if (!m_pObject)
    {
        m_pObject = new TYPE;
    }

    return m_pObject;
}

template<class TYPE> TYPE& JSingleton<TYPE>::instance2()
{
    if (!m_pObject)
    {
        m_pObject = new TYPE;
    }

    return *m_pObject;
}

template<class TYPE> TYPE* JSingleton<TYPE>::try_instance()
{
    return m_pObject;
}

#endif

