#include "gtest/gtest.h"
#include "BookingSystem.h"
#include "ErrCode.h"

class LltBookingServer{
public:
    virtual ~LltBookingServer() {}

    virtual void Setup() {
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::array<bool, MAX_SEAT_IN_THEATER>>>systems = 
        {
            {0, {{3, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
                {4, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
                {5, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}}}
            },
            {1, {{6, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
                {7, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
                {8, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}}}
            },
            {2, {{9, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
                {10, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
                {11, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}}}
            }
        };
        m_sys = new BookingServer(systems);
        if (m_sys == nullptr) {
            printf("setup failed");
        }
    }

    virtual void TearDown() {}
private:
    BookingServer* m_sys{nullptr};
};

TEST(LltBookingServer, DisplayAllMovie)
{
    ASSERT_NE(m_sys, nullptr);
    std::set<uint32_t> movieCode;
    EXPECT_EQ(ERRCODE_SUCCESS, DisplayAllMovie(movieCode));
    EXPECT_EQ(movieCode.at(0), 0);
    EXPECT_EQ(movieCode.at(1), 1);
    EXPECT_EQ(movieCode.at(2), 2);
}

TEST(LltBookingServer, DisplayAllTheatersWithSelectedMovie)
{
    ASSERT_NE(m_sys, nullptr);
    std::set<uint32_t> theaterCode;
    EXPECT_EQ(ERRCODE_SUCCESS, DisplayAllTheatersWithSelectedMovie(0, theaterCode));
    EXPECT_EQ(theaterCode.at(0), 3);
    EXPECT_EQ(theaterCode.at(1), 4);
    EXPECT_EQ(theaterCode.at(2), 5);
    theaterCode.clear();
    EXPECT_EQ(ERRCODE_SUCCESS, DisplayAllTheatersWithSelectedMovie(1, theaterCode));
    EXPECT_EQ(theaterCode.at(0), 6);
    EXPECT_EQ(theaterCode.at(1), 7);
    EXPECT_EQ(theaterCode.at(2), 8);
    theaterCode.clear();
    EXPECT_EQ(ERRCODE_SUCCESS, DisplayAllTheatersWithSelectedMovie(2, theaterCode));
    EXPECT_EQ(theaterCode.at(0), 9);
    EXPECT_EQ(theaterCode.at(1), 10);
    EXPECT_EQ(theaterCode.at(2), 11);
    theaterCode.clear();
    EXPECT_EQ(ERRCODE_INVALID_PARAMETER, DisplayAllTheatersWithSelectedMovie(3, theaterCode));
}

TEST(LltBookingServer, DisplayAllAvailableSeats)
{
    ASSERT_NE(m_sys, nullptr);
    std::array<bool, MAX_SEAT_IN_THEATER> seats;
    std::set<uint32_t>bookSeat;
    bookSeat.insert(1);
    EXPECT_EQ(ERRCODE_SUCCESS, BookSeats(0, 3, bookSeat);
    EXPECT_EQ(ERRCODE_SUCCESS, DisplayAllAvailableSeats(0, 3, seats));
    EXPECT_EQ(seats[0], false);
    EXPECT_EQ(seats[1], true);
    EXPECT_EQ(seats[2], true);
    EXPECT_EQ(ERRCODE_SEAT_IS_OCCUPIED, BookSeats(0, 3, bookSeat);
    EXPECT_EQ(ERRCODE_INVALID_PARAMETER, DisplayAllAvailableSeats(9, 3, seats));
    EXPECT_EQ(ERRCODE_INVALID_PARAMETER, DisplayAllAvailableSeats(0, 13, seats));
    EXPECT_EQ(ERRCODE_INVALID_PARAMETER, BookSeats(9, 3, bookSeat);
    EXPECT_EQ(ERRCODE_INVALID_PARAMETER, BookSeats(0, 13, bookSeat);
}