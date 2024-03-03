# ingress-playground

TCP and UDP services for deployment in Kubernetes to explore ingress

## Deployment
```bash
kubectl apply -f tcp-server-ss.yaml
kubectl apply -f udp-server-ss.yaml
```

## Minikube nginx ingress controller

Create ingress-nginx-controller-patch.yaml
```yaml
spec:
  template:
    spec:
      containers:
      - name: controller
        ports:
         - containerPort: 5000
           hostPort: 5000
           protocol: UDP
         - containerPort: 5001
           hostPort: 5001

```

Configure ingress
```bash
kubectl patch configmap udp-services -n ingress-nginx --patch '{"data":{"5000":"default/udp-server-0-service:5000"}}'
kubectl patch configmap tcp-services -n ingress-nginx --patch '{"data":{"5001":"default/tcp-server-0-service:5001"}}'
kubectl patch deployment ingress-nginx-controller -n ingress-nginx --patch "$(cat ingress-nginx-controller-patch.yaml)" -n ingress-nginx
```

Reach services on minikube ip (e.g. 192.168.49.2)

## K3S With Traefic

- On K3S master, create file `/var/lib/rancher/k3s/server/manifests/traefik-config.yaml` with additional ports to expose. See example in traefik directory

- Create instances of IngressRouteTCP or IngressRouteUDP as needed
```bash
kubectl apply -f ingressRouteTcp.yaml
kubectl apply -f ingressRouteUdp.yaml
```

- Services should be available; check details from the `traefik` service in the `kube-system` namespace, as they may be on a load balancer IP if something like `metallb` is in use.

