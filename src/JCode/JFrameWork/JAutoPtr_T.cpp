/**********************************************************
* Author: weiganyi
* File: JAutoPtr_T.cpp
* Date: 20121101
* Description: 
*
**********************************************************/

#ifndef _JAUTOPTR_T_CPP_
#define _JAUTOPTR_T_CPP_


template<class TYPE> JAutoPtrBase<TYPE>::JAutoPtrBase(TYPE* pObject)
{
    if (pObject)
    {
        m_pObject = pObject;
    }
    else
    {
        cout<< "JAutoPtrBase::JAutoPtrBase input param is NULL" << endl;
        m_pObject = NULL;
    }

    return;
}

template<class TYPE> JAutoPtrBase<TYPE>::~JAutoPtrBase()
{
    return;
}

template<class TYPE> TYPE& JAutoPtrBase<TYPE>::operator*()
{
    return *m_pObject;
}

template<class TYPE> TYPE* JAutoPtrBase<TYPE>::operator->()
{
    return m_pObject;
}

template<class TYPE> TYPE* JAutoPtrBase<TYPE>::GetObject()
{
    return m_pObject;
}

template<class TYPE> JAutoPtr<TYPE>::JAutoPtr(TYPE* pObject):JAutoPtrBase<TYPE>(pObject)
{
    return;
}

template<class TYPE> JAutoPtr<TYPE>::~JAutoPtr()
{
    if (this->GetObject())
    {
        Release();
    }

    return;
}

template<class TYPE> JAutoPtr<TYPE>& JAutoPtr<TYPE>::operator=(const JAutoPtr<TYPE>& rhs)
{
    if (this != &rhs)
    {
        //destroy the old object
        //bug: if the object pointer be used by more than one autoptr, release in here will effect
        //other autoptr.
        Release();

        //save the rhs object pointer
        this->GetObject() = rhs.GetObject();
    }

    return *this;
}

template<class TYPE> JUINT32 JAutoPtr<TYPE>::Release()
{
    delete this->GetObject();

    return JSUCCESS;
}

#endif

