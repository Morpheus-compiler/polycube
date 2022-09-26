/*
 * firewall API
 *
 * firewall API generated from firewall.yang
 *
 * API version: 1.0.0
 * Generated by: Swagger Codegen (https://github.com/swagger-api/swagger-codegen.git)
 */

package swagger

type Firewall struct {
	// Name of the firewall service
	Name string `json:"name,omitempty"`
	// UUID of the Cube
	Uuid string `json:"uuid,omitempty"`
	// Type of the Cube (TC, XDP_SKB, XDP_DRV)
	Type_ string `json:"type,omitempty"`
	ServiceName string `json:"service-name,omitempty"`
	// Logging level of a cube, from none (OFF) to the most verbose (TRACE)
	Loglevel string `json:"loglevel,omitempty"`
	// Defines if the service should be automatically optimized by Morpheus
	DynOpt bool `json:"dyn-opt,omitempty"`
	// Set this flag if you want to start Morpheus compiler
	StartMorpheus bool `json:"start-morpheus,omitempty"`
	// Enables the Connection Tracking module. Mandatory if connection tracking rules are needed. Default is ON.
	Conntrack string `json:"conntrack,omitempty"`
	// If Connection Tracking is enabled, all packets belonging to ESTABLISHED connections will be accepted automatically. Default is ON.
	AcceptEstablished string `json:"accept-established,omitempty"`
	SessionTable []SessionTable `json:"session-table,omitempty"`
	Chain []Chain `json:"chain,omitempty"`
}
