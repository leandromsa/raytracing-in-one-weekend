// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <functional>

#include "Common.h"
#include "World.h"
#include "Color.h"
#include "Sphere.h"
#include "Camera.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

Color ray_color(const Ray& r, const Hittable& world, int depth) {
    Hit hit;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color(0, 0, 0);


    if (world.hit(r, 0.001, infinity, hit)) {
        Ray scattered;
        Color attenuation;
        if (hit.material->scatter(r, hit, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return Color(0, 0, 0);
    }

    Vector3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

World random_scene() {
    World world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

void sincronous_version() {
    // Image
 //const float aspect_ratio = 16.0 / 9.0;
    const auto aspect_ratio = 3.0 / 2.0;
    //const int image_width = 256;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    //const int samples_per_pixel = 500;
    const int max_depth = 50;

    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vector3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    // World
    auto world = random_scene();

    // Camera
    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);


    // Render
    std::ofstream outputFile;
    outputFile.open("normal_canvas.ppm");
    outputFile << "P3\n" << image_width << " " << image_height << "\n255\n";
 
    for (int h = image_height - 1; h >= 0; h--) {
        std::cerr << "\rScanlines remaining: " << h << ' ' << std::flush;
        for (int w = image_width - 1; w >= 0; w--) {

            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (w + random_double()) / (image_width - 1);
                auto v = (h + random_double()) / (image_height - 1);
                Ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(outputFile, pixel_color, samples_per_pixel);

        }
    }


    outputFile.close();
    std::cerr << "\nDone.\n";

}

void omp_version() {
    // Image
       //const float aspect_ratio = 16.0 / 9.0;
    const auto aspect_ratio = 3.0 / 2.0;
    //const int image_width = 256;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    //const int samples_per_pixel = 500;
    const int max_depth = 50;

    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vector3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    // World
    auto world = random_scene();

    // Camera
    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);


    // Render
    std::vector<Color> image(image_width * image_height);


    #pragma omp parallel for num_threads(12) 
    for (int h = image_height - 1; h >= 0; h--) {
        std::cerr << "\rScanlines remaining: " << h << ' ' << std::flush;
        for (int w = image_width - 1; w >= 0; w--) {

            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (w + random_double()) / (image_width - 1);
                auto v = (h + random_double()) / (image_height - 1);
                Ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            image[h * image_width + w] = pixel_color;
        }
    }

    std::ofstream outputFile;
    outputFile.open("omp_canvas.ppm");
    outputFile << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int h = image_height - 1; h >= 0; h--) {
        std::cerr << "\rSaveing lines remaining: " << h << ' ' << std::flush;
        for (int w = image_width - 1; w >= 0; w--) {
            write_color(outputFile, image[h * image_width + w], samples_per_pixel);
        }
    }

    outputFile.close();
    std::cerr << "\nDone.\n";
}


void threads_version() {
    // Image
       //const float aspect_ratio = 16.0 / 9.0;
    const auto aspect_ratio = 3.0 / 2.0;
    //const int image_width = 256;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    //const int samples_per_pixel = 500;
    const int max_depth = 50;

    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vector3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    // World
    auto world = random_scene();

    // Camera
    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);


    // Render
    std::vector<Color> image(image_width * image_height);

    const int k_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(k_threads);

    for (int t = 0; t < k_threads; ++t) {
        threads[t] = std::thread(std::bind([&](int start, int end, int t) {
             for (int h = start; h < end; h++) {
                 std::cerr << "\rScanlines remaining: " << h << ' ' << std::flush;
                 for (int w = 0; w < image_width; w++) {
                    Color pixel_color(0, 0, 0);
                    for (int s = 0; s < samples_per_pixel; ++s) {
                        auto u = (w + random_double()) / (image_width - 1);
                        auto v = (h + random_double()) / (image_height - 1);
                        Ray r = camera.get_ray(u, v);
                        pixel_color += ray_color(r, world, max_depth);
                    }

                    image[h * image_width + w] = pixel_color;
                }
            }
            }, t* image_height / k_threads, (t + 1) == k_threads ? image_height : (t + 1) * image_height / k_threads, t));
    }

    for (int t = 0; t < k_threads; ++t) {
        threads[t].join();
    }

    std::ofstream outputFile;
    outputFile.open("threads_canvas.ppm");
    outputFile << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int h = image_height - 1; h >= 0; h--) {
        std::cerr << "\rSaveing lines remaining: " << h << ' ' << std::flush;
        for (int w = image_width - 1; w >= 0; w--) {
            write_color(outputFile, image[h * image_width + w], samples_per_pixel);
        }
    }

    outputFile.close();
    std::cerr << "\nDone.\n";
}


int main()
{

  clock_t start, stop;
  double timer_seconds;
  /*
    start = clock();
    std::cout << "Assincronous versions \n";
    sincronous_version();
    stop = clock();
    timer_seconds = ((double)(stop - start)) / CLOCKS_PER_SEC;
    std::cout << "took " << timer_seconds << " seconds.\n";

    start = clock();
    std::cout << "OMP versions \n";
    omp_version();
    stop = clock();
    timer_seconds = ((double)(stop - start)) / CLOCKS_PER_SEC;
    std::cout << "took " << timer_seconds << " seconds.\n";
    */
    start = clock();
    std::cout << "Threads versions \n";
    threads_version();
    stop = clock();
    timer_seconds = ((double)(stop - start)) / CLOCKS_PER_SEC;
    std::cout << "took " << timer_seconds << " seconds.\n";
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
