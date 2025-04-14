#include "FinalTang.hpp"

class RecommendationEngine {
private:
    TangoTree masterTree;
    std::vector<std::string> allIndustries;
    std::vector<std::string> allLocations;
    
public:
    RecommendationEngine() {
        // Initialize known industries and locations
        allIndustries = {"Tech", "Finance", "Healthcare", "Education", "Marketing"};
        allLocations = {"Lahore", "Karachi", "Islamabad"};
    }
    
    void addInternship(Internship* intern) {
        // Compute feature vector once
        
        
        // Add to master tree
        masterTree.insertInternship(intern);
    }
    
    std::vector<Internship*> getRecommendations(const UserPreferences& prefs) {
        // Convert user preferences to weights
        UserWeights weights = convertPreferencesToWeights(prefs);
        
        // Get candidate internships from the Tango Tree
        std::vector<Internship*> candidates;
        collectCandidateInternships(candidates);
        
        // Score candidates using pre-computed features
        std::vector<std::pair<float, Internship*>> scoredCandidates;
        for (Internship* intern : candidates) {
            float score = calculateUniversalScore(intern, weights);
            scoredCandidates.push_back({score, intern});
        }
        
        // Sort by score
        std::sort(scoredCandidates.begin(), scoredCandidates.end(),
                 [](const auto& a, const auto& b) { return a.first > b.first; });
        
        // Return top recommendations
        std::vector<Internship*> recommendations;
        int maxResults = 10;
        for (int i = 0; i < std::min(maxResults, (int)scoredCandidates.size()); i++) {
            recommendations.push_back(scoredCandidates[i].second);
        }
        
        return recommendations;
    }
    
    UserWeights convertPreferencesToWeights(const UserPreferences& prefs) {
        UserWeights weights;
        
        // Binary feature weights
        weights.remoteImportance = prefs.preferRemote ? 1.0f : 0.0f;
        weights.paidImportance = prefs.requirePaid ? 1.0f : 0.0f;
        weights.handsOnImportance = prefs.preferHandsOn ? 1.0f : 0.0f;
        
        // Numerical feature weights
        weights.desiredExperience = prefs.maxExperience;
        weights.experienceImportance = 0.8f;  // High importance
        weights.maxHours = prefs.maxWeeklyHours;
        weights.hoursImportance = 0.6f;  // Medium importance
        
        // Categorical preferences
        weights.industryPreference.resize(allIndustries.size(), 0.0f);
        for (size_t i = 0; i < allIndustries.size(); i++) {
            if (prefs.preferredIndustry == allIndustries[i]) {
                weights.industryPreference[i] = 1.0f;
                break;
            }
        }
        weights.industryImportance = 0.9f;  // High importance
        
        // Similar for location preferences
        // ...
        
        return weights;
    }
    
    float calculateUniversalScore(const Internship* intern, const UserWeights& weights) {
        float score = intern->relevanceScore;  // Start with base score
        
        // Apply binary feature weights
        if (intern->features.isRemote == 1) 
        score += 10 * weights.remoteImportance;
        if (intern->features.isPaid == 1) 
        score += 10 * weights.paidImportance;
        if (intern->features.isHandsOn == 1) 
        score += 10 * weights.handsOnImportance;
        
        // Apply numerical feature weights
        float expDiff = fabs(intern->features.experienceLevel - weights.desiredExperience/10.0f);
        score -= 10 * expDiff * weights.experienceImportance;  // Penalty for experience mismatch
        
        if (intern->features.hoursCommitment <= weights.maxHours/40.0f)
        score += 10 * weights.hoursImportance;
        
        // Apply categorical preferences using dot product
        float industryMatch = 0;
        for (size_t i = 0; i < intern->features.industryVector.size(); i++) {
            industryMatch += intern->features.industryVector[i] * weights.industryPreference[i];
        }
        score += 20 * industryMatch * weights.industryImportance;
        
        float locationMatch = 0;
        for (size_t i = 0; i < intern->features.locationVector.size(); i++) {
            locationMatch += intern->features.locationVector[i] * weights.locationPreference[i];
        }
        score += 15 * locationMatch * weights.locationImportance;
        
        return score;
    }
    
    void collectCandidates(std::vector<Internship*>& candidates) {
        // Use the efficient Tango Tree to collect candidates
        // This could range from searching for high relevance scores
        // to more sophisticated range queries
        for (int score = 100; score >= 70; score -= 5) {
            std::vector<Internship*> results = masterTree.search(score);
            candidates.insert(candidates.end(), results.begin(), results.end());
            if (candidates.size() >= 100) break; // Enough candidates
        }
    }


    
};
    // class RecommendationSystem {
        //     private:
        //         TangoTree masterTree;  // Single tree for all internships
        
//     public:
//         // User preferences are passed as parameters, not built into separate trees
//         std::vector<Internship*> getPersonalizedRecommendations(
//             const UserPreferences& prefs, int maxResults = 10) {
            
//             // First retrieve potential matches using the efficient Tango Tree search
//             std::vector<Internship*> candidates;
//             collectCandidates(candidates);
            
//             // Then apply personalization without tree restructuring
//             std::vector<std::pair<int, Internship*>> scoredCandidates;
//             for (Internship* intern : candidates) {
//                 if (meetsHardRequirements(intern, prefs)) {
//                     int personalizedScore = calculatePersonalizedScore(intern, prefs);
//                     scoredCandidates.push_back({personalizedScore, intern});
//                 }
//             }
            
//             // Sort and return top results
//             std::sort(scoredCandidates.begin(), scoredCandidates.end(),
//                      [](const auto& a, const auto& b) { return a.first > b.first; });
            
//             std::vector<Internship*> recommendations;
//             int count = std::min(maxResults, (int)scoredCandidates.size());
//             for (int i = 0; i < count; i++) {
//                 recommendations.push_back(scoredCandidates[i].second);
//             }
            
//             return recommendations;
//         }
        
//         // Helper method to collect candidate internships efficiently
        // void collectCandidates(std::vector<Internship*>& candidates) {
        //     // Use the efficient Tango Tree to collect candidates
        //     // This could range from searching for high relevance scores
        //     // to more sophisticated range queries
        //     for (int score = 100; score >= 70; score -= 5) {
        //         std::vector<Internship*> results = masterTree.search(score);
        //         candidates.insert(candidates.end(), results.begin(), results.end());
        //         if (candidates.size() >= 100) break; // Enough candidates
        //     }
        // }

//         std::vector<Internship*> getPersonalizedRecommendations(
//             const UserPreferences& prefs, int maxResults = 10) {
            
//             // 1. Retrieve potential matches efficiently from the Tango Tree
//             std::vector<Internship*> candidates;
//             collectCandidates(candidates);
            
//             // 2. Apply personalization filters
//             std::vector<std::pair<int, Internship*>> scoredCandidates;
//             for (Internship* intern : candidates) {
//                 // Check "hard" requirements (must-haves)
//                 if (meetsHardRequirements(intern, prefs)) {
//                     // Calculate personalized score based on preferences
//                     int personalizedScore = calculatePersonalizedScore(intern, prefs);
//                     scoredCandidates.push_back({personalizedScore, intern});
//                 }
//             }
            
//             // 3. Sort by personalized score
           
            
//             // 4. Return top results
//             std::vector<Internship*> recommendations;
//             for (int i = 0; i < std::min(maxResults, (int)scoredCandidates.size()); i++) {
//                 recommendations.push_back(scoredCandidates[i].second);
//             }
            
//             return recommendations;
//         }

//         void collectCandidates(std::vector<Internship*>& candidates) {
//             // Start with high-relevance scores and work downward
//             for (int score = 100; score >= 70; score -= 5) {
//                 // Use the Tango Tree's efficient search mechanism
//                 // This benefits from preferred paths and aux trees
//                 std::vector<Internship*> results = masterTree.search(score);
//                 candidates.insert(candidates.end(), results.begin(), results.end());
                
//                 // Exit early if we have enough candidates to filter
//                 if (candidates.size() >= 100) break;
//             }
//         }

//         bool meetsHardRequirements(Internship* intern, const UserPreferences& prefs) {
//             // Example hard requirements that must be met
//             if (prefs.requirePaid && !intern->paid) return false;
//             if (prefs.preferRemote && !intern->remote) return false;
//             if (prefs.maxExperience < intern->yearsExperience) return false;
            
//             return true;
//         }

//         int calculatePersonalizedScore(Internship* intern, const UserPreferences& prefs) {
//             int baseScore = intern->relevanceScore; // Start with base relevance
            
//             // Apply preference-based adjustments
//             if (prefs.requireRemote && intern->remote) baseScore += 20;
//             if (prefs.requirePaid && intern->paid) baseScore += 15;
            
//             // Industry match bonus (exact or related)
//             if (prefs.preferredIndustry == intern->industry) {
//                 baseScore += 25; // Exact match
//             } else if (isRelatedIndustry(prefs.preferredIndustry, intern->industry)) {
//                 baseScore += 10; // Related industry
//             }
            
//             // Experience level matching
//             int expDiff = abs(prefs.preferredExperience - intern->yearsExperience);
//             baseScore -= (expDiff * 5); // Penalty for experience mismatch
            
//             // Location preference
//             if (prefs.preferredLocation == intern->location) baseScore += 15;
            
//             // Weekly hours match
//             if (intern->weeklyHours <= prefs.maxWeeklyHours) baseScore += 10;
            
//             return baseScore;
//         }
        
//     };