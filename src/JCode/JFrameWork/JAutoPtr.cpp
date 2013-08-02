/**********************************************************
* Author: weiganyi
* File: JAutoPtr.cpp
* Date: 20121101
* Description: 
*
**********************************************************/

//implement of specialization template
template<> inline JAutoPtr<JLock>::JAutoPtr(JLock* pObject):JAutoPtrBase<JLock>(pObject)
{
    if (this->GetObject())
    {
        this->GetObject()->Acquire();
    }

    return;
}

template<> inline JAutoPtr<JLock>::~JAutoPtr()
{
    if (this->GetObject())
    {
        this->GetObject()->Release();
    }

    return;
}

