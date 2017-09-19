double return_coefficient(int u, int v){

  switch (u) {

    case 0:
    switch (v) {
      case 0:
      return 1.0/2.0;

      default:
      return 1.0/sqrt(2.0);

    }

    default:
    switch (v) {
      case 0:
      return 1.0/sqrt(2.0);

      default:
      return 1.0;
    }

  }

}
