#include <gtest/gtest.h>
#include "core/gina_action.h"
#include "core/gina_types.h"

using namespace gina;

class TestReceiver 
{
public:
    void NoArgMethod() 
    {
        callCount++; 
    }
    
    void IntArgMethod(int32 value) 
    { 
        lastInt = value; 
    }
    
    void MixedArgsMethod(int32 a, float b, const std::string& c) 
    {
        lastInt = a;
        lastFloat = b;
        lastStr = c;
    }

    int32 callCount = 0;
    int32 lastInt = 0;
    float lastFloat = 0.0f;
    std::string lastStr;
};

class ActionBasicTest : public ::testing::Test 
{
protected:
    void SetUp() override {}

    Action<> action;
    TestReceiver receiver;
};

TEST_F(ActionBasicTest, SubscribesAndInvokesNoArgMethod) 
{
    action.Subscribe(&receiver, &TestReceiver::NoArgMethod);
    
    action.Invoke();
    EXPECT_EQ(receiver.callCount, 1);
    
    action.Invoke();
    EXPECT_EQ(receiver.callCount, 2);
}

TEST_F(ActionBasicTest, HandlesMultipleInvocations) 
{
    action.Subscribe(&receiver, &TestReceiver::NoArgMethod);
    
    for (int32 i = 0; i < 5; ++i) 
    {
        action.Invoke();
    }
    EXPECT_EQ(receiver.callCount, 5);
}

class ActionWithArgsTest : public ::testing::Test 
{
protected:
    Action<int32> intAction;
    Action<int32, float, const std::string&> mixedAction;
    TestReceiver receiver;
};

TEST_F(ActionWithArgsTest, HandlesIntArgument) 
{
    intAction.Subscribe(&receiver, &TestReceiver::IntArgMethod);
    
    intAction.Invoke(42);
    EXPECT_EQ(receiver.lastInt, 42);
    
    intAction.Invoke(100);
    EXPECT_EQ(receiver.lastInt, 100);
}

TEST_F(ActionWithArgsTest, HandlesMixedArguments) 
{
    mixedAction.Subscribe(&receiver, &TestReceiver::MixedArgsMethod);
    
    mixedAction.Invoke(10, 3.14f, "test");
    EXPECT_EQ(receiver.lastInt, 10);
    EXPECT_FLOAT_EQ(receiver.lastFloat, 3.14f);
    EXPECT_EQ(receiver.lastStr, "test");
}

class ActionSubscriptionTest : public ::testing::Test 
{
protected:
    Action<> action;
    TestReceiver receiver1;
    TestReceiver receiver2;
};

TEST_F(ActionSubscriptionTest, UnsubscribeRemovesHandler) 
{
    action.Subscribe(&receiver1, &TestReceiver::NoArgMethod);
    action.Subscribe(&receiver2, &TestReceiver::NoArgMethod);
    
    action.Invoke();
    action.Unsubscribe(&receiver1);
    action.Invoke();
    
    EXPECT_EQ(receiver1.callCount, 1);
    EXPECT_EQ(receiver2.callCount, 2);
}

TEST_F(ActionSubscriptionTest, ClearRemovesAllHandlers) 
{
    action.Subscribe(&receiver1, &TestReceiver::NoArgMethod);
    action.Subscribe(&receiver2, &TestReceiver::NoArgMethod);
    
    action.Clear();
    action.Invoke();
    
    EXPECT_EQ(receiver1.callCount, 0);
    EXPECT_EQ(receiver2.callCount, 0);
}

TEST(ActionEdgeCasesTest, InvokeWithoutSubscribersDoesNotThrow) 
{
    Action<int32> action;
    EXPECT_NO_THROW(action.Invoke(42));
}

TEST(ActionEdgeCasesTest, UnsubscribeNonExistingDoesNotThrow) 
{
    Action<> action;
    TestReceiver receiver;
    EXPECT_NO_THROW(action.Unsubscribe(&receiver));
}