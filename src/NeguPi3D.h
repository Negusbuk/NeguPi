/****************************************************************************
 **
 **  Copyright (C) 2019 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#ifndef NEGUPI3D_H
#define NEGUPI3D_H

#include <cstdint>
#include <cmath>

namespace NeguPi {

  template <typename T> class Quaternion
  {
  public:

    Quaternion()
    : w_(0), x_(0), y_(0), z_(0) { }

    Quaternion(T w, T x, T y, T z)
    : w_(w), x_(x), y_(y), z_(z) { }

    const T w() const { return w_; }
    T& w() { return w_; }

    const T x() const { return x_; }
    T& x() { return x_; }

    const T y() const { return y_; }
    T& y() { return y_; }

    const T z() const { return z_; }
    T& z() { return z_; }

    Quaternion<T> getProduct(const Quaternion<T>& q) const {
      // Quaternion multiplication is defined by:
      //     (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
      //     (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
      //     (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
      //     (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2
      return Quaternion<T>(w_*q.w() - x_*q.x() - y_*q.y() - z_*q.z(),  // new w
                           w_*q.x() + x_*q.w() + y_*q.z() - z_*q.y(),  // new x
                           w_*q.y() - x_*q.z() + y_*q.w() + z_*q.x(),  // new y
                           w_*q.z() + x_*q.y() - y_*q.x() + z_*q.w()); // new z
    }

    Quaternion<T> getConjugate() const {
      return Quaternion<T>(w_, -x_, -y_, -z_);
    }

    double getMagnitude() {
      return std::sqrt(w_*w_ + x_*x_ + y_*y_ + z_*z_);
    }

    void normalize() {
      double m = getMagnitude();
      w_ /= m;
      x_ /= m;
      y_ /= m;
      z_ /= m;
    }

    Quaternion<T> getNormalized() {
      Quaternion<T> r(w_, x_, y_, z_);
      r.normalize();
      return r;
    }

  protected:

    T w_;
    T x_;
    T y_;
    T z_;
  };

  typedef Quaternion<int16_t> QuaternionInt16;
  typedef Quaternion<int32_t> QuaternionInt32;
  typedef Quaternion<double> QuaternionDouble;

  template <typename T> class Vector3D
  {
    public:

    Vector3D()
    : x_(0), y_(0), z_(0) { }

    Vector3D(T x, T y, T z)
    : x_(x), y_(y), z_(z) { }

    const T& x() const { return x_; }
    T& x() { return x_; }

    const T& y() const { return y_; }
    T& y() { return y_; }

    const T& z() const { return z_; }
    T& z() { return z_; }

    double getMagnitude() {
      return std::sqrt(x_*x_ + y_*y_ + z_*z_);
    }

    void normalize() {
      double m = getMagnitude();
      x_ /= m;
      y_ /= m;
      z_ /= m;
    }

    Vector3D<T> getNormalized() {
      Vector3D<T> r(x_, y_, z_);
      r.normalize();
      return r;
    }

    void rotate(const Quaternion<T>& q) {
      // http://www.cprogramming.com/tutorial/3d/quaternions.html
      // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
      // http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
      // ^ or: http://webcache.googleusercontent.com/search?q=cache:xgJAp3bDNhQJ:content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation&hl=en&gl=us&strip=1

      // P_out = q * P_in * conj(q)
      // - P_out is the output vector
      // - q is the orientation quaternion
      // - P_in is the input vector (a*aReal)
      // - conj(q) is the conjugate of the orientation quaternion (q=[w,x,y,z], q*=[w,-x,-y,-z])
      Quaternion<T> p(0, x_, y_, z_);

      // quaternion multiplication: q * p, stored back in p
      p = q.getProduct(p);

      // quaternion multiplication: p * conj(q), stored back in p
      p = p.getProduct(q.getConjugate());

      // p quaternion is now [0, x', y', z']
      x_ = p.x();
      y_ = p.y();
      z_ = p.z();
    }

    Vector3D<T> getRotated(const Quaternion<T>& q) {
      Vector3D<T> r(x_, y_, z_);
      r.rotate(q);
      return r;
    }

    protected:

      T x_;
      T y_;
      T z_;
  };

  typedef Vector3D<int16_t> VectorInt16;
  typedef Vector3D<int32_t> VectorInt32;
  typedef Vector3D<double> VectorDouble;
};

#endif
