/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CCOBJECT_H__
#define __CCOBJECT_H__

#include "CCDataVisitor.h"
#include "ccMacros.h"
#include <unordered_map>

#ifdef EMSCRIPTEN
#include <GLES2/gl2.h>
#endif // EMSCRIPTEN

RT_ADD(
    class DS_Dictionary;
)

NS_CC_BEGIN

/**
 * @addtogroup base_nodes
 * @{
 */

RT_ADD(
    typedef enum {
        kCCObjectTypePlayLayer = 5,
        kCCObjectTypeLevelEditorLayer = 6,
        kCCObjectTypeMenuLayer = 15,
    } CCObjectType;
)

class CCZone;
class CCObject;
class CCNode;
class CCEvent;

/**
 * This class is used to fix the problem of destructor recursion.
 */
GEODE_ADD(
	class GEODE_CC_DLL CCDestructor {
	private:
		static inline auto& destructorLock() {
			static std::unordered_map<void*, bool> ret;
			return ret;
		}
	public:
		static inline bool& lock(void* self) {
			return destructorLock()[self];
		}
		inline ~CCDestructor() {
			destructorLock().erase(this);
		}
	};
)

/**
 * @js NA
 * @lua NA
 */
class CC_DLL CCCopying GEODE_ADD(: public CCDestructor)
{
    GEODE_ADD(friend struct geode::interfaces;)
public:
    virtual CCObject* copyWithZone(CCZone* pZone)  { return 0; }
};

/**
 * @js NA
 */
class CC_DLL CCObject : public CCCopying
{
    GEODE_ADD(friend struct geode::interfaces;)
public:
    // object id, CCScriptSupport need public m_uID
    unsigned int        m_uID;
    // Lua reference id
    int                 m_nLuaID;
protected:
    // the object's tag
    RT_ADD( int m_nTag; )
    // count of references
    unsigned int        m_uReference;
    // count of autorelease
    unsigned int        m_uAutoReleaseCount;

    RT_ADD(
        CCObjectType m_eObjType;
        int m_nUnknown;
    )
public:
    CCObject(void);
    /**
     *  @lua NA
     */
    virtual ~CCObject(void);
    
    void release(void);
    void retain(void);
    CCObject* autorelease(void);
    CCObject* copy(void);
    bool isSingleReference(void) const;
    unsigned int retainCount(void) const;
    virtual bool isEqual(const CCObject* pObject);

    virtual void acceptVisitor(CCDataVisitor &visitor);

    virtual void update(float dt) {CC_UNUSED_PARAM(dt);};
    
    RT_ADD(
        virtual void encodeWithCoder(DS_Dictionary*);

        static CCObject* createWithCoder(DS_Dictionary*);
        
        virtual bool canEncode();

        CCObjectType getObjType() const;
        
        virtual int getTag() const;

        virtual void setTag(int nTag);
       
        inline void setObjType(CCObjectType type) {
        	m_eObjType = type;
        }
    
        //i have no idea if vtable function order matters so 
    )

    friend class CCAutoreleasePool;
};


typedef void (CCObject::*SEL_SCHEDULE)(float);
typedef void (CCObject::*SEL_CallFunc)();
typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
typedef void (CCObject::*SEL_CallFuncND)(CCNode*, void*);
typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
typedef void (CCObject::*SEL_MenuHandler)(CCObject*);
typedef void (CCObject::*SEL_EventHandler)(CCEvent*);
typedef int (CCObject::*SEL_Compare)(CCObject*);

#define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
#define callfunc_selector(_SELECTOR) (SEL_CallFunc)(&_SELECTOR)
#define callfuncN_selector(_SELECTOR) (SEL_CallFuncN)(&_SELECTOR)
#define callfuncND_selector(_SELECTOR) (SEL_CallFuncND)(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) (SEL_CallFuncO)(&_SELECTOR)
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
#define event_selector(_SELECTOR) (SEL_EventHandler)(&_SELECTOR)
#define compare_selector(_SELECTOR) (SEL_Compare)(&_SELECTOR)

// end of base_nodes group
/// @}

NS_CC_END

#endif // __CCOBJECT_H__
