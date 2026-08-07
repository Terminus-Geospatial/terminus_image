/**
 * @file    TEST_Compound_Types.cpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/types/Compound_Types.hpp>
#include <terminus/image/pixel/Pixel_RGBA.hpp>

// C++ Libraries
#include <type_traits>

namespace tix = tmns::image;
namespace tmx = tmns::math;

/**
 * A simple compound type.  We only test with this one type here, and thus
 * we do not exercise all of the specialized code paths.  However, those
 * do largely get exercized by the pixel math tests in the Image module.
 */
template <class ChannelT>
class Test_Compound
{
    public:

        /**
         * Constructor
        */
        Test_Compound( ChannelT a, ChannelT b ) { values[0]=a; values[1]=b; }

        /**
         * Index Operator
        */
        ChannelT& operator[]( size_t i ) { return values[i]; }

        /**
         * Index Reference Operator
         */
        ChannelT const& operator[]( size_t i ) const { return values[i]; }

    private:

        ChannelT values[2];
};

template<class ChannelT>
struct tmns::math::Compound_Channel_Type<Test_Compound<ChannelT> >
{
    typedef ChannelT type;
};

template<class ChannelT>
struct tmns::math::Compound_Channel_Count<Test_Compound<ChannelT> >
{
    static constexpr size_t value() { return 2; };
};


template<class InT, class OutT>
struct tmns::math::Compound_Channel_Cast<Test_Compound<InT>, OutT>
{
    typedef Test_Compound<OutT> type;
};

// A dummy type that is neither a scalar nor a compound type.
class Dummy_Type {};

/******************************************/
/*      Test the Compound Name Method     */
/******************************************/
TEST( Compound_Types, Compound_Name )
{
    // Check Compound Names
    ASSERT_EQ( tmx::Compound_Name<tmns::image::PixelRGBA_u8>::name(),  "uint8_t" );
    
}


/*
// Simple functions to test the compound_apply logic.
template <class T> T add( T const& a, T const& b ) { return boost::numeric_cast<T>(a + b); }
template <class T> void add_in_place( T& a, T const& b ) { a += b; }
template <class T> T add_one( T const& val ) { return val + 1; }
template <class T> void add_one_in_place( T& val ) { val += 1; }


namespace vw {
  template<class ChannelT> struct CompoundChannelType<Test_Compound<ChannelT> > { typedef ChannelT type; };
  template<class ChannelT> struct CompoundNumChannels<Test_Compound<ChannelT> > { static const size_t value = 2; };
  template<class InT, class OutT> struct CompoundChannelCast<Test_Compound<InT>, OutT> { typedef Test_Compound<OutT> type; };
}


template <class T1, class T2>
static bool is_of_type( T2 ) {
  return boost::is_same<T1,T2>::value;
}

TEST(CompoundTypes, Traits) {

  EXPECT_TRUE(( boost::is_same<CompoundAccumulatorType<uint8>::type,AccumulatorType<uint8>::type>::value ));
  EXPECT_TRUE(( boost::is_same<CompoundAccumulatorType<Test_Compound<uint8> >::type,Test_Compound<AccumulatorType<uint8>::type> >::value ));
  EXPECT_TRUE(( boost::is_same<CompoundAccumulatorType<const uint8>::type,AccumulatorType<uint8>::type>::value ));
  EXPECT_TRUE(( boost::is_same<CompoundAccumulatorType<const Test_Compound<uint8> >::type,Test_Compound<AccumulatorType<uint8>::type> >::value ));
}

TEST(CompoundTypes, CompoundSelectChannel) {
  uint32 vali = 3;
  EXPECT_EQ( 3u, compound_select_channel<const uint32&>(vali,0) );
  compound_select_channel<uint32&>(vali,0) = 5;
  EXPECT_EQ( 5u, compound_select_channel<const uint32&>(vali,0) );
  float valf = 4.0;
  EXPECT_EQ( 4.0, compound_select_channel<const float&>(valf,0) );
  compound_select_channel<float&>(valf,0) = 6;
  EXPECT_EQ( 6.0, compound_select_channel<const float&>(valf,0) );
  Test_Compound<uint32> valci( 1, 2 );
  EXPECT_EQ( 1u, compound_select_channel<const uint32&>(valci,0) );
  EXPECT_EQ( 2u, compound_select_channel<const uint32&>(valci,1) );
  compound_select_channel<uint32&>(valci,0) = 3;
  compound_select_channel<uint32&>(valci,1) = 4;
  EXPECT_EQ( 3u, compound_select_channel<const uint32&>(valci,0) );
  EXPECT_EQ( 4u, compound_select_channel<const uint32&>(valci,1) );
  Test_Compound<float> valcf( 2.0, 3.0 );
  EXPECT_EQ( 2.0, compound_select_channel<const float&>(valcf,0) );
  EXPECT_EQ( 3.0, compound_select_channel<const float&>(valcf,1) );
  compound_select_channel<float&>(valcf,0) = 3.0;
  compound_select_channel<float&>(valcf,1) = 4.0;
  EXPECT_EQ( 3.0, compound_select_channel<const float&>(valcf,0) );
  EXPECT_EQ( 4.0, compound_select_channel<const float&>(valcf,1) );
}

TEST(CompoundTypes, BinaryCompoundApply) {
  uint32 ai=1, bi=2, ci=compound_apply(&add<uint32>, ai, bi);
  EXPECT_EQ( 3u, ci );
  float af=1.0, bf=2.0, cf=compound_apply(&add<float>, af, bf);
  EXPECT_EQ( 3.0, cf );
  Test_Compound<uint32> aci(1,2), bci(3,4), cci=compound_apply(&add<uint32>, aci, bci);
  EXPECT_EQ( 4u, cci[0] );
  EXPECT_EQ( 6u, cci[1] );
  Test_Compound<float> acf(1,2), bcf(3,4), ccf=compound_apply(&add<float>, acf, bcf);
  EXPECT_EQ( 4, ccf[0] );
  EXPECT_EQ( 6, ccf[1] );
}

TEST(CompoundTypes, BinaryCompoundApplyInPlace) {
  uint32 ai=1, bi=2;
  compound_apply_in_place(&add_in_place<uint32>, ai, bi);
  EXPECT_EQ( 3u, ai );
  float af=1.0, bf=2.0;
  compound_apply_in_place(&add_in_place<float>, af, bf);
  EXPECT_EQ( 3.0, af );
  Test_Compound<uint32> aci(1,2), bci(3,4);
  compound_apply_in_place(&add_in_place<uint32>, aci, bci);
  EXPECT_EQ( 4u, aci[0] );
  EXPECT_EQ( 6u, aci[1] );
  Test_Compound<float> acf(1,2), bcf(3,4);
  compound_apply_in_place(&add_in_place<float>, acf, bcf);
  EXPECT_EQ( 4, acf[0] );
  EXPECT_EQ( 6, acf[1] );
}

TEST(CompoundTypes, UnaryCompoundApply) {
  uint32 ai=1, bi=compound_apply(&add_one<uint32>, ai);
  EXPECT_EQ( 2u, bi );
  float af=1.0, bf=compound_apply(&add_one<float>, af);
  EXPECT_EQ( 2.0, bf );
  Test_Compound<uint32> aci(1,2), bci=compound_apply(&add_one<uint32>, aci);
  EXPECT_EQ( 2u, bci[0] );
  EXPECT_EQ( 3u, bci[1] );
  Test_Compound<float> acf(1,2), bcf=compound_apply(&add_one<float>, acf);
  EXPECT_EQ( 2, bcf[0] );
  EXPECT_EQ( 3, bcf[1] );
}

TEST( CompoundTypes, UnaryCompoundApplyInPlace) {
  uint32 vali = 0;
  compound_apply_in_place( &add_one_in_place<uint32>, vali );
  EXPECT_EQ( 1.0, vali );
  double valf = 0.0;
  compound_apply_in_place( &add_one_in_place<double>, valf );
  EXPECT_EQ( 1.0, valf );
  Test_Compound<int> valc( 1, 2 );
  compound_apply_in_place( &add_one_in_place<int>, valc );
  EXPECT_EQ( 2, valc[0] );
  EXPECT_EQ( 3, valc[1] );
}
*/
