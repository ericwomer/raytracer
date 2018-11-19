/* Ignore this for now
hitable *random_scene() {
  int n = 500;
  hitable **list = new hitable *[n + 1];
  list[0] =
      new sphere(Vec3(0, -1000, 0), 1000, new lambertian(Vec3(0.5, 0.5, 0.5)));

  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      double choosen_mat = drand48();
      Vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
      if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
        if (choosen_mat < 0.8) {
          list[i++] = new sphere(
              center, 0.2,
              new lambertian(Vec3(drand48() * drand48(), drand48() * drand48(),
                                  drand48() * drand48())));
        } else if (choosen_mat < 0.95) {
          list[i++] = new sphere(
              center, 0.2,
              new metal(Vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()),
                             0.5 * (1 + drand48())),
                        0.5 * drand48()));
        } else {
          list[i++] = new sphere(center, 0.2, new dielectric(1.5));
        }
      }
    }
  }
  list[i++] = new sphere(Vec3(0, 1, 0), 1.0, new dielectric(1.5));
  list[i++] =
      new sphere(Vec3(-4, 1, 0), 1.0, new lambertian(Vec3(0.4, 0.2, 0.1)));
  list[i++] =
      new sphere(Vec3(4, 1, 0), 1.0, new metal(Vec3(0.7, 0.6, 0.0), 0.0));

  return new hitable_list(list, i);
}
*/

/**
 * @brief Eventually turn this in to a valid progress indicator.
 *
 */
/*
class pBar {
public:
  void update(double newProgress) {
    currentProgress += newProgress;
    amountOfFiller =
        (int)((currentProgress / neededProgress) * (double)pBarLength);
  }
  void print() {
    currUpdateVal %= pBarUpdater.length();
    std::cout << "\r"                          // Bring cursor to start of line
              << firstPartOfpBar;              // Print out first part of pBar
    for (int a = 0; a < amountOfFiller; a++) { // Print out current progress
      std::cout << pBarFiller;
    }
    std::cout << pBarUpdater[currUpdateVal];
    for (int b = 0; b < pBarLength - amountOfFiller; b++) { // Print out spaces
      std::cout << " ";
    }
    std::cout << lastPartOfpBar // Print out last part of progress bar
              << " (" << (int)(100 * (currentProgress / neededProgress))
              << "%)" // This just prints out the percent
              << std::flush;
    currUpdateVal += 1;
  }
  std::string firstPartOfpBar =
                  "[", // Change these at will (that is why I made them public)
      lastPartOfpBar = "]", pBarFiller = "|", pBarUpdater = "/-\\|";

private:
  int amountOfFiller,
      pBarLength = 50,        // I would recommend NOT changing this
      currUpdateVal = 0;      // Do not change
  double currentProgress = 0, // Do not change
      neededProgress = 100;   // I would recommend NOT changing this
}; */