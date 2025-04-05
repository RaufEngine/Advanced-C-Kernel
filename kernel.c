/* Include necessary libraries */
#include <stdio.h>    // For basic input/output
#include <stdlib.h>   // For memory allocation and random numbers
#include <math.h>     // For math functions (sqrt)
#include <time.h>     // For random seed
#include <string.h>   // For memory operations (memset)

/* Function to calculate distance between a pixel and a centroid */
float calculate_distance(float* pixel, float* centroid) {
    // Calculate squared differences for each color channel
    float diff_r = pixel[0] - centroid[0];
    float diff_g = pixel[1] - centroid[1];
    float diff_b = pixel[2] - centroid[2];
    
    // Return squared Euclidean distance (we skip sqrt for performance)
    return diff_r * diff_r + diff_g * diff_g + diff_b * diff_b;
}

/* Function to initialize centroids randomly */
void initialize_centroids(float* pixels, int num_pixels, float* centroids, int num_centroids, int seed) {
    // Set random seed for reproducibility
    srand(seed);
    
    // For each centroid
    for (int i = 0; i < num_centroids; i++) {
        // Pick a random pixel index
        int random_idx = rand() % num_pixels;
        
        // Copy that pixel's RGB values to be our centroid
        centroids[i*3] = pixels[random_idx*3];      // Red
        centroids[i*3+1] = pixels[random_idx*3+1];  // Green
        centroids[i*3+2] = pixels[random_idx*3+2];  // Blue
    }
}

/* Main k-means clustering function */
void kmeans_clustering(float* pixels, int num_pixels, int num_centroids, int max_iters, int seed, float* centroids, int* labels) {
    // 1. Initialize centroids randomly
    initialize_centroids(pixels, num_pixels, centroids, num_centroids, seed);
    
    // Allocate temporary arrays
    int* counts = (int*)malloc(num_centroids * sizeof(int)); // Count of pixels per cluster
    float* sums = (float*)malloc(num_centroids * 3 * sizeof(float)); // Sum of RGB values per cluster
    
    // 2. Run for specified iterations
    for (int iter = 0; iter < max_iters; iter++) {
        // Reset counts and sums to zero
        memset(counts, 0, num_centroids * sizeof(int));
        memset(sums, 0, num_centroids * 3 * sizeof(float));
        
        // 3. Assign each pixel to nearest centroid
        for (int i = 0; i < num_pixels; i++) {
            float* pixel = &pixels[i*3]; // Current pixel (RGB)
            int closest = 0; // Start assuming first centroid is closest
            float min_dist = calculate_distance(pixel, &centroids[0]);
            
            // Compare with all other centroids
            for (int j = 1; j < num_centroids; j++) {
                float dist = calculate_distance(pixel, &centroids[j*3]);
                if (dist < min_dist) {
                    min_dist = dist;
                    closest = j;
                }
            }
            
            // Update labels and cluster statistics
            labels[i] = closest;
            counts[closest]++;
            sums[closest*3] += pixel[0];   // Sum Red
            sums[closest*3+1] += pixel[1]; // Sum Green
            sums[closest*3+2] += pixel[2]; // Sum Blue
        }
        
        // 4. Update centroids to be mean of their assigned pixels
        for (int j = 0; j < num_centroids; j++) {
            if (counts[j] > 0) { // Only update if cluster has pixels
                centroids[j*3] = sums[j*3] / counts[j];      // New Red mean
                centroids[j*3+1] = sums[j*3+1] / counts[j];  // New Green mean
                centroids[j*3+2] = sums[j*3+2] / counts[j]; // New Blue mean
            }
        }
    }
    
    // Free allocated memory
    free(counts);
    free(sums);
}