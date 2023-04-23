#version 330 core

#define MAX_STEPS 100
#define MAX_DIST 50.
#define SURF_DIST .01

out vec4 o_frag_color;
uniform int u_supersample;
uniform vec3 u_camera_pos;
uniform vec3 u_camera_front;
uniform vec3 u_camera_up;
uniform vec3 u_camera_right;
uniform float u_fov;
uniform float u_time;
uniform vec2 u_resolution;

float get_dist(vec3 pos) {
  int iterations = 10;
  float bailout = 3.0;

  vec4 z = vec4(pos, 1.0);
  float dr = 1.0;
  float r = 0.0;

  for (int i = 0; i < iterations; ++i) {
    r = length(z.xyz);
    if (r > bailout) break;

    // Convert to polar coordinates
    float theta = acos(z.z / r);
    float phi = atan(z.y, z.x);
    dr = pow(r, 7.0) * 8.0 * dr + 1.0;

    // Scale and rotate the point
    float zr = pow(r, 8.0);
    theta = theta * 8.0;
    phi = phi * 8.0;

    // Convert back to cartesian coordinates
    z = zr * vec4(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta), 0.0) + vec4(pos, 1.0);
  }
  return min(0.5 * log(r) * r / dr, pos.y + 2);
}

vec3 get_normal(vec3 p) {
  float d = get_dist(p);
  vec2 e = vec2(0.001, 0);

  vec3 n = d - vec3(get_dist(p - e.xyy),
                    get_dist(p - e.yxy),
                    get_dist(p - e.yyx));

  return normalize(n);
}

float ray_march(vec3 ro, vec3 rd) {
  float dO = 0.0;

  for(int i = 0; i < MAX_STEPS; i++) {
    if (dO > MAX_DIST) break;
    vec3 p = ro + rd * dO;
    float dS = get_dist(p);
    dO += dS;
    if (dO < SURF_DIST) {
      return dO;
    }
  }

  return dO;
}

float get_light(vec3 p) {
  vec3 light_pos = vec3(0, 5, 6);
  light_pos.xz += vec2(sin(10), cos(10))*16.;
  vec3 l = normalize(light_pos - p);
  vec3 n = get_normal(p);

  float dif = clamp(dot(n, l), 0.0, 1.0);
  float d = ray_march(p + n * SURF_DIST * 2.0, l);
  if (d < length(light_pos - p)) dif *= 0.1;

  return dif;
}

void main() {
  if (u_supersample == 1) {
  float aspect_ratio = u_resolution.x / u_resolution.y;
  float scale = tan(radians(u_fov * 0.5));
  vec2 uv_offset4x[4] = vec2[](vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(-0.5, 0.5), vec2(0.5, 0.5));
  vec2 uv_offset[8] = vec2[](
      vec2(-0.25, -0.25), vec2(0.25, -0.25), vec2(-0.25, 0.25), vec2(0.25, 0.25),
      vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(-0.5, 0.5), vec2(0.5, 0.5));
  vec2 uv_offset16x[16] = vec2[](
      vec2(-0.25, -0.25), vec2(0.25, -0.25), vec2(-0.25, 0.25), vec2(0.25, 0.25),
      vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(-0.5, 0.5), vec2(0.5, 0.5),
      vec2(-0.75, -0.75), vec2(0.75, -0.75), vec2(-0.75, 0.75), vec2(0.75, 0.75),
      vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0));
  int num_samples = 8;
  vec3 color = vec3(0.0);
  for (int i = 0; i < num_samples; ++i) {
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy + uv_offset[i] * 0.5) / u_resolution.y;
    vec3 ro = u_camera_pos;

    vec3 rd = normalize(
      u_camera_right * uv.x * scale * aspect_ratio + u_camera_up * uv.y * scale + u_camera_front);

    float d = ray_march(ro, rd);
    vec3 p = ro + rd * d;
    float dif = get_light(p);

    color += vec3(dif);
  }

  color /= num_samples;
  o_frag_color = vec4(pow(color, vec3(0.4545)), 1.0);
} else {
  vec3 color = vec3(0.0);
  float aspect_ratio = u_resolution.x / u_resolution.y;
  float scale = tan(radians(u_fov * 0.5));

  vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy) / u_resolution.y;
  vec3 ro = u_camera_pos;

  vec3 rd = normalize(
    u_camera_right * uv.x * scale * aspect_ratio + u_camera_up * uv.y * scale + u_camera_front);

  float d = ray_march(ro, rd);
  vec3 p = ro + rd * d;
  float dif = get_light(p);

  color += pow(vec3(dif), vec3(0.4545));
  float a = d / MAX_DIST;
  color += (1.0 - a) * color * 0.3 + a * vec3(0.2f, 0.3f, 0.3f) * 0.7;

  o_frag_color = vec4(color, 1.0);
}
}
