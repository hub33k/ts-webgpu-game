import type { Resource, ResourceLoader, ResourceType } from '.';

export class ResourceManager {
  private resources: Resource[] = [];
  private loaders = new Map<ResourceType, ResourceLoader<Resource>>();

  public addLoader<T extends Resource>(
    resourceType: ResourceType,
    loader: ResourceLoader<T>,
  ): void {
    this.loaders.set(resourceType, loader);
  }

  public async load(url: string, resourceType: ResourceType): Promise<void> {
    const loader = this.loaders.get(resourceType);
    if (!loader) {
      throw new Error(`No loader found for resourceType: ${resourceType}`);
    }

    const resource = await loader(url);
    this.resources.push(resource);
  }

  public get(url: string): Resource | undefined {
    return this.resources.find((resource) => resource.id === url);
  }

  public getAll(resourceType: ResourceType): Resource[] {
    return this.resources.filter((resource) => resource.type === resourceType);
  }
}
