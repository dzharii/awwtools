#include "aww-inspiration.hpp"
#include <array>
#include <cstdlib>
#include <ctime>
#include <string_view>

namespace aww::inspiration {
// array of strings with predefined inspirational quotes
constexpr std::array quotes = {
    "I trust in my abilities and the power of my words.",
    "I am worthy of success and will put in the necessary work to achieve it.",
    "I am open to new ideas and perspectives.",
    "I will not be limited by fear or self-doubt.",
    "I am capable of creating something valuable and meaningful.",
    "I choose to focus on the positive and let go of negative thoughts.",
    "I am grateful for the opportunity to express myself through writing.",
    "Every word I write brings me closer to my goals.",
    "I am determined to succeed and will not let obstacles stand in my way.",
    "I will finish this document with confidence and pride in my work.",
    "I will take action now.",
    "I will not let distractions get in the way of my progress.",
    "I will break this task into manageable chunks.",
    "I will set a deadline and work towards it.",
    "I will focus on the benefits of completing this task.",
    "I will remind myself of the consequences of not taking action.",
    "I will prioritize this task and make it a priority.",
    "I will eliminate any unnecessary steps and simplify the task.",
    "I will not let perfectionism hold me back.",
    "I will take the first step and keep moving forward.",
    "I will approach this problem with an open mind.",
    "I will consider all possible solutions.",
    "I will gather all necessary information before making a decision.",
    "I will think critically and objectively about the problem.",
    "I will ask for input and advice from others.",
    "I will not rush to a conclusion, but take the time to evaluate all options.",
    "I will break the problem down into smaller parts to make it more manageable.",
    "I will look at the problem from different perspectives.",
    "I will not be afraid to challenge my assumptions and question my own thinking.",
    "I will trust my intuition and creativity to find a solution.",
    "I will stay laser-focused on my goal.",
    "I will eliminate distractions and stay on task.",
    "I will prioritize the most important tasks and stay organized.",
    "I will stay committed to finding a solution.",
    "I will maintain a positive attitude and remain motivated.",
    "I will visualize the successful outcome and stay motivated",
    "I will remind myself why this problem and goal are important to me.",
    "I will take regular breaks to refresh my mind and stay productive.",
    "I will set small and achievable milestones to keep moving forward.",
    "I will remind myself that hard work and determination will pay off in the end.",
    "I will embrace my unique voice and perspective.",
    "I will allow myself to think outside the box.",
    "I will be open to new ideas and inspiration.",
    "I will trust my instincts and not be afraid to take risks.",
    "I will not be afraid of failure, but see it as an opportunity to learn and grow.",
    "I will practice self-compassion and be kind to myself.",
    "I will remind myself that great ideas often come from making mistakes",
    "I will seek out new experiences and perspectives to fuel my creativity.",
    "I will take time for self-reflection and listen to my inner voice.",
    "I will believe in myself and my ability to create something valuable.",
};

constexpr size_t quotesLen = quotes.size();

std::string getInspiration(void) {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  const std::size_t randomIndex =
      static_cast<std::size_t>(static_cast<unsigned int>(std::rand()) % quotesLen);
  return quotes[randomIndex];
}
} // namespace aww::inspiration
